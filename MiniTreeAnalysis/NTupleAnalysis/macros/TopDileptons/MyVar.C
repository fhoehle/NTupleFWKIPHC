#include <iomanip>
#include <iostream>
#include <limits.h>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>


///////////////////////////////////////////////////////////////////////////////////
//
// Utilisation: changer le type de channel etudie (ee/mumu/emu)
// Modifier le fichier de config ../../config/MyCutFlow.xml afferant (pour emu mettre METCutsEMU a 0 ou 20)
//
///////////////////////////////////////////////////////////////////////////////////

//void  EffCompute(TH1F& ,TH1F& ,TH1F& ,TH1F& , TGraph& );
void  EffCompute(TH1F& ,TH1F& ,TH1F& ,TH1F& , float[], float[] );

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
  DiLeptonSelection sel; 
  float Luminosity = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/MyVar.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  TopTree::NTEvent * event = 0;

  //dataset weights
  double Dweight[101]; 
  for(int k0=0; k0<5; ++k0) {
    for(int k1=0; k1<101; ++k1) {
      Dweight[k1] = 0.;
    }
  } 
  vector<string> CutName;
  vector<string> VecChannelName;
  


// Here define the studied channel (ee/mumu/emu)
  string ChannelName  = "mumu";  // "mumu", "ee", "emu"


  int ITypeMC	  = -1;
  int ICut	  = -1;  
  int IChannel    = -1;  
  if (  ChannelName  == "mumu" )  IChannel = 0;
  if (  ChannelName  == "ee" )    IChannel = 1;
  if (  ChannelName  == "emu" )   IChannel = 2;

  
  
  int nbins = 200;
  float minx = 0.;
  float maxx = 350;
  
  
  cout<<"The verbosity mode is "<<verbosity <<endl;
  cout<<"The luminosity is equal to "<< Luminosity<<endl;
  cout<<"The DataType is ";
  switch(DataType){
  case 0: 
    cout<<"MC"<<endl; 
    break;
  case 1:
    cout<<"Data"<<endl; 
    break;
  case 2:
    cout<<"Data & MC"<<endl; 
    break;
  default:
    cout<<" unknown"<<endl; 
    break;
  }
  //////////////////////
  
    HistoManager MyhistoManager;
    MyhistoManager.LoadDatasets(datasets);   
     

//     std::vector<TH1F> PATElecPt;
//     std::vector<TH1F> PATElecRelIso;
//     std::vector<TH1F> PATMuonPt;
//     std::vector<TH1F> PATMuonRelIso;
    
    std::vector<TH1F> PFElecPt;
    std::vector<TH1F> PFElecNeutralHadIso;
    std::vector<TH1F> PFElecChargedHadIso;
    std::vector<TH1F> PFElecPhotonIso;
    std::vector<TH1F> PFElecTrackIso;
    std::vector<TH1F> PFElecRelIso;
    std::vector<TH1F> PFElecEcalDriven;
    std::vector<TH1F> PFElecGsfEl;
    std::vector<TH1F> PFElecD0;
    std::vector<TH1F> PFElecId90;
    std::vector<TH1F> PFElecConv;
    std::vector<TH1F> PFElecRelIsoRhoCorrected;
    std::vector<TH1F> PFElecRelIsoNeutralRhoCorrected;
    
    
    std::vector<TH1F> PFMuonPt;
    std::vector<TH1F> PFMuonNeutralHadIso;
    std::vector<TH1F> PFMuonChargedHadIso;
    std::vector<TH1F> PFMuonPhotonIso;
    std::vector<TH1F> PFMuonTrackIso;
    std::vector<TH1F> PFMuonRelIso;
    std::vector<TH1F> PFMuonType;
    std::vector<TH1F> PFMuonChi2;
    std::vector<TH1F> PFMuonTrValHit;
    std::vector<TH1F> PFMuonValHit;
    std::vector<TH1F> PFMuonD0;
    std::vector<TH1F> PFMuonZvx;
    std::vector<TH1F> PFMuonRelIsoRhoCorrected;
    std::vector<TH1F> PFMuonRelIsoNeutralRhoCorrected;

    std::vector<TH1F> TMEME;
    
//     std::vector<TH1F> PATElecPt_FromW;
//     std::vector<TH1F> PATElecRelIso_FromW;
//     std::vector<TH1F> PATMuonPt_FromW;
//     std::vector<TH1F> PATMuonRelIso_FromW;
//     std::vector<TH1F> PATElecPt_FromW_Cut015;
//     std::vector<TH1F> PATMuonPt_FromW_Cut015;
    
    std::vector<TH1F> PFElecPt_FromW;
    std::vector<TH1F> PFElecNeutralHadIso_FromW;
    std::vector<TH1F> PFElecChargedHadIso_FromW;
    std::vector<TH1F> PFElecPhotonIso_FromW;
    std::vector<TH1F> PFElecTrackIso_FromW;
    std::vector<TH1F> PFElecRelIso_FromW;
    
    std::vector<TH1F> PFMuonPt_FromW;
    std::vector<TH1F> PFMuonNeutralHadIso_FromW;
    std::vector<TH1F> PFMuonChargedHadIso_FromW;
    std::vector<TH1F> PFMuonPhotonIso_FromW;
    std::vector<TH1F> PFMuonTrackIso_FromW;
    std::vector<TH1F> PFMuonRelIso_FromW;
    std::vector<TH1F> PFElecPt_FromW_Cut015;
    std::vector<TH1F> PFMuonPt_FromW_Cut015;
    std::vector<TH1F> PFMuonType_FromW;
    
    
//     std::vector<TH1F> PATElecPt_FromBC;
//     std::vector<TH1F> PATElecRelIso_FromBC;
//     std::vector<TH1F> PATMuonPt_FromBC;
//     std::vector<TH1F> PATMuonRelIso_FromBC;
//     std::vector<TH1F> PATElecPt_FromBC_Cut015;
//     std::vector<TH1F> PATMuonPt_FromBC_Cut015;
    
    std::vector<TH1F> PFElecPt_FromBC;
    std::vector<TH1F> PFElecNeutralHadIso_FromBC;
    std::vector<TH1F> PFElecChargedHadIso_FromBC;
    std::vector<TH1F> PFElecPhotonIso_FromBC;
    std::vector<TH1F> PFElecTrackIso_FromBC;
    std::vector<TH1F> PFElecRelIso_FromBC;
    
    std::vector<TH1F> PFMuonPt_FromBC;
    std::vector<TH1F> PFMuonNeutralHadIso_FromBC;
    std::vector<TH1F> PFMuonChargedHadIso_FromBC;
    std::vector<TH1F> PFMuonPhotonIso_FromBC;
    std::vector<TH1F> PFMuonTrackIso_FromBC;
    std::vector<TH1F> PFMuonRelIso_FromBC;
    std::vector<TH1F> PFElecPt_FromBC_Cut015;
    std::vector<TH1F> PFMuonPt_FromBC_Cut015;
    std::vector<TH1F> PFMuonType_FromBC;
    
  
  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Loop over the datasets  "<<endl;
    cout<<"#########################"<<endl;
  }
  

  for (unsigned int d = 0; d < datasets.size (); d++) {
    
    
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    
//     MyhistoManager.CreateHisto(PATElecPt,                   "PATElecPt" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATElecRelIso,               "PATElecRelIso" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
//     MyhistoManager.CreateHisto(PATMuonPt,                   "PATMuonPt" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATMuonRelIso,               "PATMuonRelIso" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);

    MyhistoManager.CreateHisto(PFElecPt,                   "PFElecPt" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFElecNeutralHadIso,               "PFElecNeutralHadIso"   ,datasets[d].Name(),"NeutralHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecChargedHadIso,               "PFElecChargedHadIso" ,datasets[d].Name(),"ChargedHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecPhotonIso,               "PFElecPhotonIso" ,datasets[d].Name(),"PhotonIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecTrackIso,               "PFElecTrackIso" ,datasets[d].Name(),"TrackIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecRelIso,               "PFElecRelIso" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(PFElecEcalDriven,               "PFElecEcalDriven" ,datasets[d].Name(),"isEcalDriven","Entries",2,-0.5,1.5);
    MyhistoManager.CreateHisto(PFElecGsfEl,               "PFElecGsfEl" ,datasets[d].Name(),"isGsfEl","Entries",2,-0.5,1.5);
    MyhistoManager.CreateHisto(PFElecD0,               "PFElecD0" ,datasets[d].Name(),"D0","Entries",50,0.,1.);
    MyhistoManager.CreateHisto(PFElecId90,               "PFElecId90" ,datasets[d].Name(),"isId90","Entries",2,-0.5,1.5);
    MyhistoManager.CreateHisto(PFElecConv,               "PFElecConv" ,datasets[d].Name(),"isConv","Entries",2,-0.5,1.5);
    MyhistoManager.CreateHisto(PFElecRelIsoRhoCorrected,               "PFElecRelIsoRhoCorrected" ,datasets[d].Name(),"RelIsoRhoCorrected","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(PFElecRelIsoNeutralRhoCorrected,               "PFElecRelIsoNeutralRhoCorrected",datasets[d].Name(),"RelIsoNeutralRhoCorrected","Entries",50,0.,1.0);

    MyhistoManager.CreateHisto(PFMuonPt,                   "PFMuonPt" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFMuonNeutralHadIso,               "PFMuonNeutralHadIso" ,datasets[d].Name(),"NeutralHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonChargedHadIso,               "PFMuonChargedHadIso" ,datasets[d].Name(),"ChargedHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonPhotonIso,               "PFMuonPhotonIso" ,datasets[d].Name(),"PhotonIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonTrackIso,               "PFMuonTrackIso" ,datasets[d].Name(),"TrackIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonRelIso,               "PFMuonRelIso" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(TMEME,                   "TMEME" ,datasets[d].Name(),"TMEME","Entries",1000,0.,30000);
    MyhistoManager.CreateHisto(PFMuonType,               "PFMuonType" ,datasets[d].Name(),"Type","Entries",120,0.,119);
    MyhistoManager.CreateHisto(PFMuonChi2,               "PFMuonChi2" ,datasets[d].Name(),"Chi2","Entries",50,0.,50);
    MyhistoManager.CreateHisto(PFMuonTrValHit,               "PFMuonTrValHit" ,datasets[d].Name(),"TrValHit","Entries",50,0.,50);
    MyhistoManager.CreateHisto(PFMuonValHit,               "PFMuonValHit" ,datasets[d].Name(),"ValHit","Entries",50,0.,50);
    MyhistoManager.CreateHisto(PFMuonD0,               "PFMuonD0" ,datasets[d].Name(),"D0","Entries",50,0.,1.);
    MyhistoManager.CreateHisto(PFMuonZvx,               "PFMuonZvx" ,datasets[d].Name(),"D0","Entries",50,0.,1);
    MyhistoManager.CreateHisto(PFMuonRelIsoRhoCorrected,               "PFMuonRelIsoRhoCorrected" ,datasets[d].Name(),"RelIsoRhoCorrected","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(PFMuonRelIsoNeutralRhoCorrected,               "PFMuonRelIsoNeutralRhoCorrected",datasets[d].Name(),"RelIsoNeutralRhoCorrected","Entries",50,0.,1.0);
    
//     MyhistoManager.CreateHisto(PATElecPt_FromW,                   "PATElecPt_FromW" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATElecRelIso_FromW,               "PATElecRelIso_FromW" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
//     MyhistoManager.CreateHisto(PATMuonPt_FromW,                   "PATMuonPt_FromW" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATMuonRelIso_FromW,               "PATMuonRelIso_FromW" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
//     MyhistoManager.CreateHisto(PATElecPt_FromW_Cut015,                   "PATElecPt_FromW_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATMuonPt_FromW_Cut015,                   "PATMuonPt_FromW_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);

    MyhistoManager.CreateHisto(PFElecPt_FromW,                   "PFElecPt_FromW" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFElecNeutralHadIso_FromW,               "PFElecNeutralHadIso_FromW"   ,datasets[d].Name(),"NeutralHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecChargedHadIso_FromW,               "PFElecChargedHadIso_FromW" ,datasets[d].Name(),"ChargedHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecPhotonIso_FromW,               "PFElecPhotonIso_FromW" ,datasets[d].Name(),"PhotonIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecTrackIso_FromW,               "PFElecTrackIso_FromW" ,datasets[d].Name(),"TrackIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecRelIso_FromW,               "PFElecRelIso_FromW" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(PFMuonPt_FromW,                   "PFMuonPt_FromW" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFMuonNeutralHadIso_FromW,               "PFMuonNeutralHadIso_FromW" ,datasets[d].Name(),"NeutralHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonChargedHadIso_FromW,               "PFMuonChargedHadIso_FromW" ,datasets[d].Name(),"ChargedHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonPhotonIso_FromW,               "PFMuonPhotonIso_FromW" ,datasets[d].Name(),"PhotonIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonTrackIso_FromW,               "PFMuonTrackIso_FromW" ,datasets[d].Name(),"TrackIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonRelIso_FromW,               "PFMuonRelIso_FromW" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(PFElecPt_FromW_Cut015,                   "PFElecPt_FromW_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFMuonPt_FromW_Cut015,                   "PFMuonPt_FromW_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFMuonType_FromW,               "PFMuonType_FromW" ,datasets[d].Name(),"Type","Entries",120,0.,119);

//     MyhistoManager.CreateHisto(PATElecPt_FromBC,                   "PATElecPt_FromBC" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATElecRelIso_FromBC,               "PATElecRelIso_FromBC" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
//     MyhistoManager.CreateHisto(PATMuonPt_FromBC,                   "PATMuonPt_FromBC" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATMuonRelIso_FromBC,               "PATMuonRelIso_FromBC" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
//     MyhistoManager.CreateHisto(PATElecPt_FromBC_Cut015,                   "PATElecPt_FromBC_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
//     MyhistoManager.CreateHisto(PATMuonPt_FromBC_Cut015,                   "PATMuonPt_FromBC_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);

    MyhistoManager.CreateHisto(PFElecPt_FromBC,                   "PFElecPt_FromBC" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFElecNeutralHadIso_FromBC,               "PFElecNeutralHadIso_FromBC"   ,datasets[d].Name(),"NeutralHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecChargedHadIso_FromBC,               "PFElecChargedHadIso_FromBC" ,datasets[d].Name(),"ChargedHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecPhotonIso_FromBC,               "PFElecPhotonIso_FromBC" ,datasets[d].Name(),"PhotonIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecTrackIso_FromBC,               "PFElecTrackIso_FromBC" ,datasets[d].Name(),"TrackIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFElecRelIso_FromBC,               "PFElecRelIso_FromBC" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(PFMuonPt_FromBC,                   "PFMuonPt_FromBC" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFMuonNeutralHadIso_FromBC,               "PFMuonNeutralHadIso_FromBC" ,datasets[d].Name(),"NeutralHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonChargedHadIso_FromBC,               "PFMuonChargedHadIso_FromBC" ,datasets[d].Name(),"ChargedHadIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonPhotonIso_FromBC,               "PFMuonPhotonIso_FromBC" ,datasets[d].Name(),"PhotonIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonTrackIso_FromBC,               "PFMuonTrackIso_FromBC" ,datasets[d].Name(),"TrackIso","Entries",100,0.,10.0);
    MyhistoManager.CreateHisto(PFMuonRelIso_FromBC,               "PFMuonRelIso_FromBC" ,datasets[d].Name(),"RelIso","Entries",50,0.,1.0);
    MyhistoManager.CreateHisto(PFElecPt_FromBC_Cut015,                   "PFElecPt_FromBC_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFMuonPt_FromBC_Cut015,                   "PFMuonPt_FromBC_Cut015" ,datasets[d].Name(),"Pt","Entries",50,0.,100);
    MyhistoManager.CreateHisto(PFMuonType_FromBC,               "PFMuonType_FromBC" ,datasets[d].Name(),"Type","Entries",120,0.,199);


    //////////////////////
    //LOOP OVER THE EVENTS
    //////////////////////
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
      
      //      cout << "event number=" << event->eventNb << endl;
      //      cout << "normfactor=" << datasets[d].NormFactor() << endl;
      //      cout << "Xsection=" << datasets[d].Xsection() << endl;
      //      cout << "NofEvtsToRunOver=" << datasets[d].NofEvtsToRunOver() << endl;
      
      
      MyhistoManager.FillHisto(TMEME, "TMEME",event->TMEME,datasets[d].Name(),true,1.0);

      if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
      //if(ievt > 50000) break;
      //Load event for the selection
      sel.LoadEvent(event);
      
      //Collection of selected objects
      vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
      vector<NTElectron> selElectrons = sel.GetSelectedElectronsNoIso();
      vector<NTMuon>     selMuons     = sel.GetSelectedMuonsNoIso();
      vector<NTJet>      selJets      = sel.GetSelectedJets();
      NTMET met                       = sel.GetMET(); 
      
      
      //Candidate pair of lepton
      string CandType; // ee - emu - mumum or false
      vector<NTElectron> candElec;
      vector<NTMuon> candMuon;
      
      sel.GetLeptonPair(candMuon, candElec, CandType ); 
      
      TLorentzVector DiLepton_mumu;
      for(unsigned int i=0;i<candMuon.size();i++) DiLepton_mumu+=candMuon[i].p4;
      TLorentzVector DiLepton_ee;
      for(unsigned int i=0;i<candElec.size();i++) DiLepton_ee+=candElec[i].p4;
      TLorentzVector DiLepton_emu;
      for(unsigned int i=0;i<candMuon.size();i++) DiLepton_emu+=candMuon[i].p4;
      for(unsigned int i=0;i<candElec.size();i++) DiLepton_emu+=candElec[i].p4;

      float InvDilMass = 0;
      if (ChannelName=="mumu")  InvDilMass = DiLepton_mumu.M();
      if (ChannelName=="ee")	InvDilMass = DiLepton_ee.M();
      if (ChannelName=="emu")	InvDilMass = DiLepton_emu.M();
      
      
// keep only Z mass peak
      if ( InvDilMass<76 ||  InvDilMass>106 ) continue;     
      
      
      
      //Manage DY samples to avoid overlaps
      double dileptInvMass = 0;
      if( (event->zAndDecays).size() > 0){
        TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
	dileptInvMass = dilept.M();
      }
      
      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50 ) cout << "problem !!!" << endl;
       
      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50) continue;
      
      if(datasets[d].Name()=="DYToMuMu_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToEE_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToTauTau_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToEE_M-10To20"	   &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToTauTau_M-10To20" &&  dileptInvMass > 20) continue;
      
      
     
      double weightITypeMC = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
      
      
      if ( datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMu") weightITypeMC = 1;
      bool IsSignal = false;
      int LastStep = 0;
      bool IsData = false;
      
      if ( datasets[d].Name()=="TTbar" ) {
        if ( ChannelName=="mumu" ) {
	  if ( event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000 )    IsSignal = true;
	  if ( !(event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000) ) IsSignal = false;
	}  
        if ( ChannelName=="ee" ) {
	  if ( event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200 )     IsSignal = true;
	  if ( !(event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200) )  IsSignal = false;
	}  
        if ( ChannelName=="emu" ) {
	  if ( event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110 )     IsSignal = true;
	  if ( !(event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110) )  IsSignal = false;
	}  
      }
      
      
      if ( datasets[d].Name()=="TTbar" ) { 
        if (IsSignal ) { ITypeMC = 0;  Dweight[ITypeMC]= weightITypeMC; 
                       } 
	else { ITypeMC = 1; Dweight[ITypeMC]= weightITypeMC; 
             }
      }
      if ( datasets[d].Name()=="Zjets"              || 
	   datasets[d].Name()=="DYToMuMu_M-20"      || 
	   datasets[d].Name()=="DYToEE_M-20"        || 
	   datasets[d].Name()=="DYToTauTau_M-20"    || 
	   datasets[d].Name()=="DYToMuMu_M-10To20"  || 
	   datasets[d].Name()=="DYToEE_M-10To20"    || 
	   datasets[d].Name()=="DYToTauTau_M-10To20"
	   ) { 
        ITypeMC = 2; IsSignal = false; Dweight[ITypeMC]= weightITypeMC;  
      }
      if ( datasets[d].Name()=="WjetsMu" ||
           datasets[d].Name()=="WjetsE"  ||
           datasets[d].Name()=="WjetsTau" 
       ) { 
        ITypeMC = 3; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      }
      if ( datasets[d].Name()=="SingleToptW" ) { 
        ITypeMC = 4; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      }
      if ( datasets[d].Name()=="WZ" || datasets[d].Name()=="WW" || datasets[d].Name()=="ZZ"  ) { 
        ITypeMC = 5; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      }
      if ( datasets[d].Name()=="DYee" ) { 
        ITypeMC = 6; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      }
      if ( datasets[d].Name()=="DYmumu" ) { 
        ITypeMC = 7; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      }
      if ( datasets[d].Name()=="QCD1" ||
           datasets[d].Name()=="QCD2" ||
           datasets[d].Name()=="QCD3" ||
           datasets[d].Name()=="QCD4" ||
           datasets[d].Name()=="QCD5" ||
           datasets[d].Name()=="QCD6" ||
           datasets[d].Name()=="QCD7" ||
           datasets[d].Name()=="QCD8" ||
           datasets[d].Name()=="QCD9" ||
           datasets[d].Name()=="QCD10" ||
           datasets[d].Name()=="QCD11" ||
           datasets[d].Name()=="QCD12" ||
           datasets[d].Name()=="QCD13" 
      
        ) { 
        ITypeMC = 8; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC;
      }
      if ( datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMu") { 
        ITypeMC = 100; IsData = true;  Dweight[ITypeMC]= weightITypeMC; 
      }
      
      sel.passTriggerSelection ( &datasets[d], ChannelName);
      
      for(unsigned int i=0;i<selElectrons.size();i++){
         NTElectron el = selElectrons[i];   
 
//        vector<NTElectron> allElec = event->electrons;
//       for(unsigned int i=0;i<allElec.size();i++){
//          NTElectron el = allElec[i];
	 
//	 weight = 1.0;
	 
         IsSignal = false;
         if ( event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200 ||event->TMEME==11 || event->TMEME==21100 ||
	      event->TMEME==11001 || event->TMEME==10110 ) IsSignal = true;
	      
    
// 	 MyhistoManager.FillHisto(PATElecPt, "PATElecPt",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	 MyhistoManager.FillHisto(PATElecRelIso, "PATElecRelIso",el.CombinedRelIso03(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
    
	 MyhistoManager.FillHisto(PFElecPt, "PFElecPt",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecNeutralHadIso, "PFElecNeutralHadIso",el.PATNeutralHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecChargedHadIso, "PFElecChargedHadIso",el.PATChargedHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecPhotonIso, "PFElecPhotonIso",el.PATPhotonIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecTrackIso, "PFElecTrackIso",el.PATTrackIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecRelIso, "PFElecRelIso",(el.PATTrackIso+el.PATChargedHadronIso+el.PATNeutralHadronIso)/el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
         int isEcalDriven = (el.isEcalDriven ==1);
	 MyhistoManager.FillHisto(PFElecEcalDriven, "PFElecEcalDriven",isEcalDriven,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
         int isGsfEl = (el.isGsfElectron ==1);
	 MyhistoManager.FillHisto(PFElecGsfEl, "PFElecGsfEl",isGsfEl,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecD0, "PFElecD0",el.D0,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
         int isId90 = 0;
	 if ( el.elecIdWP90_r & 0x1 ) isId90 =1;  
	 MyhistoManager.FillHisto(PFElecId90, "PFElecId90",isId90,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
         int isConv = 1;
	 if (el.elecIdWP90_r & 0x4 ) isConv =0;  
	 MyhistoManager.FillHisto(PFElecConv, "PFElecConv",isConv,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecRelIsoRhoCorrected, "PFElecRelIsoRhoCorrected",el.RelIso03PFRhoCorrected(event->rho_PUUE_dens),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFElecRelIsoNeutralRhoCorrected, "PFElecRelIsoNeutralRhoCorrected",el.RelIso03PFNeutralRhoCorrected(event->rho_PUUE_dens),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);

         if ( el.LeptonOrigin == 1 ) {
//    	    MyhistoManager.FillHisto(PATElecPt_FromW, "PATElecPt_FromW",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    MyhistoManager.FillHisto(PATElecRelIso_FromW, "PATElecRelIso_FromW",el.CombinedRelIso03(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
    
	    MyhistoManager.FillHisto(PFElecPt_FromW, "PFElecPt_FromW",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecNeutralHadIso_FromW, "PFElecNeutralHadIso_FromW",el.PATNeutralHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecChargedHadIso_FromW, "PFElecChargedHadIso_FromW",el.PATChargedHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecPhotonIso_FromW, "PFElecPhotonIso_FromW",el.PATPhotonIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecTrackIso_FromW, "PFElecTrackIso_FromW",el.PATTrackIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecRelIso_FromW, "PFElecRelIso_FromW",(el.PATTrackIso+el.PATChargedHadronIso+el.PATNeutralHadronIso)/el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
//    	    if (el.CombinedRelIso03()<0.15) { 
// 	       MyhistoManager.FillHisto(PATElecPt_FromW_Cut015, "PATElecPt_FromW_Cut015",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    }   
   	    if ((el.PATTrackIso+el.PATChargedHadronIso+el.PATNeutralHadronIso)/el.p4.Pt()<0.15) { 
	       MyhistoManager.FillHisto(PFElecPt_FromW_Cut015, "PFElecPt_FromW_Cut015",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    }   
         }

         if ( el.LeptonOrigin >= 100 ) {
//    	    MyhistoManager.FillHisto(PATElecPt_FromBC, "PATElecPt_FromBC",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    MyhistoManager.FillHisto(PATElecRelIso_FromBC, "PATElecRelIso_FromBC",el.CombinedRelIso03(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
    
	    MyhistoManager.FillHisto(PFElecPt_FromBC, "PFElecPt_FromBC",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecNeutralHadIso_FromBC, "PFElecNeutralHadIso_FromBC",el.PATNeutralHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecChargedHadIso_FromBC, "PFElecChargedHadIso_FromBC",el.PATChargedHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecPhotonIso_FromBC, "PFElecPhotonIso_FromBC",el.PATPhotonIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecTrackIso_FromBC, "PFElecTrackIso_FromBC",el.PATTrackIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFElecRelIso_FromBC, "PFElecRelIso_FromBC",(el.PATTrackIso+el.PATChargedHadronIso+el.PATNeutralHadronIso)/el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
//    	    if (el.CombinedRelIso03()<0.15) { 
// 	       MyhistoManager.FillHisto(PATElecPt_FromBC_Cut015, "PATElecPt_FromBC_Cut015",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    }   
   	    if ((el.PATTrackIso+el.PATChargedHadronIso+el.PATNeutralHadronIso)/el.p4.Pt()<0.15) { 
	       MyhistoManager.FillHisto(PFElecPt_FromBC_Cut015, "PFElecPt_FromBC_Cut015",el.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    }   
         }

//     std::cout<<"PFElecRelIso "<<(el.PATTrackIso+el.PATChargedHadronIso+el.PATNeutralHadronIso)/el.p4.Pt()<<std::endl;
//     std::cout<<"el.PATTrackIso "<<(el.PATTrackIso)<<std::endl;
//     std::cout<<"el.PATChargedHadronIso "<<(el.PATChargedHadronIso)<<std::endl;
//     std::cout<<"el.PATNeutralHadronIso "<<(el.PATNeutralHadronIso)<<std::endl;
//     std::cout<<"el.PATPhotonIso "<<(el.PATPhotonIso)<<std::endl;
//     if ( el.p4.Pt()>10 ) std::cout<<"el.LeptonOrigin "<<(el.LeptonOrigin)<<std::endl;

      }	 

       for(unsigned int i=0;i<selMuons.size();i++){
          NTMuon mu = selMuons[i];
    
//        vector<NTMuon> allMuon = event->muons;
//       for(unsigned int i=0;i<allMuon.size();i++){
//           NTMuon mu = allMuon[i];
    
//	 weight = 1.0;
	 
         IsSignal = false;
         if ( event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000 ||event->TMEME==11 || event->TMEME==21100 ||
	      event->TMEME==11001 || event->TMEME==10110 ) IsSignal=true;
	      

// 	 MyhistoManager.FillHisto(PATMuonPt, "PATMuonPt",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	 MyhistoManager.FillHisto(PATMuonRelIso, "PATMuonRelIso",mu.RelIso03(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);

	 MyhistoManager.FillHisto(PFMuonPt, "PFMuonPt",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonNeutralHadIso, "PFMuonNeutralHadIso",mu.PATNeutralHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonChargedHadIso, "PFMuonChargedHadIso",mu.PATChargedHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonPhotonIso, "PFMuonPhotonIso",mu.PATPhotonIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonTrackIso, "PFMuonTrackIso",mu.PATTrackIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonRelIso, "PFMuonRelIso",(mu.PATTrackIso+mu.PATChargedHadronIso+mu.PATNeutralHadronIso)/mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
         int isMuon = 0 ;
	 if ( mu.MuonType>=100 ) isMuon += 1;
	 if ( (mu.MuonType-(mu.MuonType/10)*10) !=0 ) isMuon += 10 ;
	 if ( mu.MuonType>=100 && (mu.MuonType-(mu.MuonType/10)*10) !=0 ) isMuon += 100;
	 MyhistoManager.FillHisto(PFMuonType, "PFMuonType",isMuon,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonChi2, "PFMuonChi2",mu.Chi2,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonTrValHit, "PFMuonTrValHit",mu.NTrValidHits,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonValHit, "PFMuonValHit",mu.NValidHits,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonD0, "PFMuonD0",mu.D0Inner,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 if (selVertices.size()>0 ) MyhistoManager.FillHisto(PFMuonZvx, "PFMuonZvx",fabs( mu.vertex.Z() - selVertices[0].p3.Z()),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonRelIsoRhoCorrected,"PFMuonRelIsoRhoCorrected",mu.RelIso03PFRhoCorrected(event->rho_PUUE_dens),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	 MyhistoManager.FillHisto(PFMuonRelIsoNeutralRhoCorrected, "PFMuonRelIsoNeutralRhoCorrected",mu.RelIso03PFNeutralRhoCorrected(event->rho_PUUE_dens),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
    
         if ( mu.LeptonOrigin == 1 ) {
// 	    MyhistoManager.FillHisto(PATMuonPt_FromW, "PATMuonPt_FromW",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    MyhistoManager.FillHisto(PATMuonRelIso_FromW, "PATMuonRelIso_FromW",mu.RelIso03(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);

	    MyhistoManager.FillHisto(PFMuonPt_FromW, "PFMuonPt_FromW",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonNeutralHadIso_FromW, "PFMuonNeutralHadIso_FromW",mu.PATNeutralHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonChargedHadIso_FromW, "PFMuonChargedHadIso_FromW",mu.PATChargedHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonPhotonIso_FromW, "PFMuonPhotonIso_FromW",mu.PATPhotonIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonTrackIso_FromW, "PFMuonTrackIso_FromW",mu.PATTrackIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonRelIso_FromW, "PFMuonRelIso_FromW",(mu.PATTrackIso+mu.PATChargedHadronIso+mu.PATNeutralHadronIso)/mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
            int isMuon = 0 ;
	    if ( mu.MuonType>=100 ) isMuon += 1;
	    if ( (mu.MuonType-(mu.MuonType/10)*10) !=0 ) isMuon += 10 ;
	    if ( mu.MuonType>=100 && (mu.MuonType-(mu.MuonType/10)*10) !=0 ) isMuon += 100;
	    MyhistoManager.FillHisto(PFMuonType_FromW, "PFMuonType_FromW",isMuon,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    
//    	    if (mu.RelIso03()<0.15) { 
// 	       MyhistoManager.FillHisto(PATMuonPt_FromW_Cut015, "PATMuonPt_FromW_Cut015",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    }   
   	    if ((mu.PATTrackIso+mu.PATChargedHadronIso+mu.PATNeutralHadronIso)/mu.p4.Pt()<0.15) { 
	       MyhistoManager.FillHisto(PFMuonPt_FromW_Cut015, "PFMuonPt_FromW_Cut015",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    }   
         }

         if ( mu.LeptonOrigin >= 100 ) {
// 	    MyhistoManager.FillHisto(PATMuonPt_FromBC, "PATMuonPt_FromBC",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    MyhistoManager.FillHisto(PATMuonRelIso_FromBC, "PATMuonRelIso_FromBC",mu.RelIso03(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);

	    MyhistoManager.FillHisto(PFMuonPt_FromBC, "PFMuonPt_FromBC",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonNeutralHadIso_FromBC, "PFMuonNeutralHadIso_FromBC",mu.PATNeutralHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonChargedHadIso_FromBC, "PFMuonChargedHadIso_FromBC",mu.PATChargedHadronIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonPhotonIso_FromBC, "PFMuonPhotonIso_FromBC",mu.PATPhotonIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonTrackIso_FromBC, "PFMuonTrackIso_FromBC",mu.PATTrackIso,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(PFMuonRelIso_FromBC, "PFMuonRelIso_FromBC",(mu.PATTrackIso+mu.PATChargedHadronIso+mu.PATNeutralHadronIso)/mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
            int isMuon = 0 ;
	    if ( mu.MuonType>=100 ) isMuon += 1;
	    if ( (mu.MuonType-(mu.MuonType/10)*10) !=0 ) isMuon += 10 ;
	    if ( mu.MuonType>=100 && (mu.MuonType-(mu.MuonType/10)*10) !=0 ) isMuon += 100;
	    MyhistoManager.FillHisto(PFMuonType_FromBC, "PFMuonType_FromBC",isMuon,datasets[d].Name(),IsSignal,Dweight[ITypeMC]);

//    	    if (mu.RelIso03()<0.15) { 
// 	       MyhistoManager.FillHisto(PATMuonPt_FromBC_Cut015, "PATMuonPt_FromBC_Cut015",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
// 	    }   
   	    if ((mu.PATTrackIso+mu.PATChargedHadronIso+mu.PATNeutralHadronIso)/mu.p4.Pt()<0.15) { 
	       MyhistoManager.FillHisto(PFMuonPt_FromBC_Cut015, "PFMuonPt_FromBC_Cut015",mu.p4.Pt(),datasets[d].Name(),IsSignal,Dweight[ITypeMC]);
	    }   
         }

//     std::cout<<"PFMuonRelIso "<<(mu.PATTrackIso+mu.PATChargedHadronIso+mu.PATNeutralHadronIso)/mu.p4.Pt()<<std::endl;
//     std::cout<<"mu.PATTrackIso "<<(mu.PATTrackIso)<<std::endl;
//     std::cout<<"mu.PATChargedHadronIso "<<(mu.PATChargedHadronIso)<<std::endl;
//     std::cout<<"mu.PATNeutralHadronIso "<<(mu.PATNeutralHadronIso)<<std::endl;
//     std::cout<<"mu.PATPhotonIso "<<(mu.PATPhotonIso)<<std::endl;
      }	 

	  
      
      
    }  // end of loop over evts
    
    
  }				// end of loop over the datasets 
  
  
// // PF electron
//     TH1F PFElecRelIsoEff_FromW_TTbarSig ("PFElecRelIsoEff_FromW_TTbarSig","PFElecRelIsoEff_FromW_TTbarSig",50,0.,1.0);
//     TH1F PFElecRelIsoEff_FromBC_TTbarBkg ("PFElecRelIsoEff_FromBC_TTbarBkg","PFElecRelIsoEff_FromBC_TTbarBkg",50,0.,1.0);
//     TH1F h1 ("h1","h1",50,0.,1.0);
//     TH1F h2 ("h2","h2",50,0.,1.0);
//     MyhistoManager.GetHisto(PFElecRelIso_FromW,"PFElecRelIso_FromW_TTbarSig",h1);
//     MyhistoManager.GetHisto(PFElecRelIso_FromBC,"PFElecRelIso_FromBC_TTbarBkg",h2);
//     
//     const int maxbin=200;
//     float X[maxbin];
//     float Y[maxbin];
//     EffCompute(h1,PFElecRelIsoEff_FromW_TTbarSig,h2,PFElecRelIsoEff_FromBC_TTbarBkg,X,Y);
//     TGraph* PFElecRelIsoPerf = new TGraph(h1.GetNbinsX(), X, Y);
//     PFElecRelIsoPerf->SetName("PFElecRelIsoPerf");
   
// // PAT electron
//     TH1F PATElecRelIsoEff_FromW_TTbarSig ("PATElecRelIsoEff_FromW_TTbarSig","PATElecRelIsoEff_FromW_TTbarSig",50,0.,1.0);
//     TH1F PATElecRelIsoEff_FromBC_TTbarBkg ("PATElecRelIsoEff_FromBC_TTbarBkg","PATElecRelIsoEff_FromBC_TTbarBkg",50,0.,1.0);
//     TH1F h3 ("h3","h3",50,0.,1.0);
//     TH1F h4 ("h4","h4",50,0.,1.0);
//     MyhistoManager.GetHisto(PATElecRelIso_FromW,"PATElecRelIso_FromW_TTbarSig",h3);
//     MyhistoManager.GetHisto(PATElecRelIso_FromBC,"PATElecRelIso_FromBC_TTbarBkg",h4);
//     
//     EffCompute(h3,PATElecRelIsoEff_FromW_TTbarSig,h4,PATElecRelIsoEff_FromBC_TTbarBkg,X,Y);
//     TGraph* PATElecRelIsoPerf = new TGraph(h3.GetNbinsX(), X, Y);
//     PATElecRelIsoPerf->SetName("PATElecRelIsoPerf");
   
// // PF muon
//     TH1F PFMuonRelIsoEff_FromW_TTbarSig ("PFMuonRelIsoEff_FromW_TTbarSig","PFMuonRelIsoEff_FromW_TTbarSig",50,0.,1.0);
//     TH1F PFMuonRelIsoEff_FromBC_TTbarBkg ("PFMuonRelIsoEff_FromBC_TTbarBkg","PFMuonRelIsoEff_FromBC_TTbarBkg",50,0.,1.0);
//     TH1F h5 ("h5","h5",50,0.,1.0);
//     TH1F h6 ("h6","h6",50,0.,1.0);
//     MyhistoManager.GetHisto(PFMuonRelIso_FromW,"PFMuonRelIso_FromW_TTbarSig",h5);
//     MyhistoManager.GetHisto(PFMuonRelIso_FromBC,"PFMuonRelIso_FromBC_TTbarBkg",h6);
//     
//     EffCompute(h5,PFMuonRelIsoEff_FromW_TTbarSig,h6,PFMuonRelIsoEff_FromBC_TTbarBkg,X,Y);
//     TGraph* PFMuonRelIsoPerf = new TGraph(h5.GetNbinsX(), X, Y);
//     PFMuonRelIsoPerf->SetName("PFMuonRelIsoPerf");
   
// // PAT muon
//     TH1F PATMuonRelIsoEff_FromW_TTbarSig ("PATMuonRelIsoEff_FromW_TTbarSig","PATMuonRelIsoEff_FromW_TTbarSig",50,0.,1.0);
//     TH1F PATMuonRelIsoEff_FromBC_TTbarBkg ("PATMuonRelIsoEff_FromBC_TTbarBkg","PATMuonRelIsoEff_FromBC_TTbarBkg",50,0.,1.0);
//     TH1F h7 ("h7","h7",50,0.,1.0);
//     TH1F h8 ("h8","h8",50,0.,1.0);
//     MyhistoManager.GetHisto(PATMuonRelIso_FromW,"PATMuonRelIso_FromW_TTbarSig",h7);
//     MyhistoManager.GetHisto(PATMuonRelIso_FromBC,"PATMuonRelIso_FromBC_TTbarBkg",h8);
//     
//     EffCompute(h7,PATMuonRelIsoEff_FromW_TTbarSig,h8,PATMuonRelIsoEff_FromBC_TTbarBkg,X,Y);
//     TGraph* PATMuonRelIsoPerf = new TGraph(h7.GetNbinsX(), X, Y);
//     PATMuonRelIsoPerf->SetName("PATMuonRelIsoPerf");
   
  
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Write output root file "<<endl;
    cout<<"#########################"<<endl;
  }
  string ofilename = string("MyVar")+string("_")+ChannelName+string(".root");
  TFile* fout  = new TFile(ofilename.c_str(),"RECREATE");
     MyhistoManager.WriteMyHisto(TMEME,"all");
     
//      MyhistoManager.WriteMyHisto(PATElecPt,"all");
//      MyhistoManager.WriteMyHisto(PATElecRelIso,"all");
//      MyhistoManager.WriteMyHisto(PATMuonPt,"all");
//      MyhistoManager.WriteMyHisto(PATMuonRelIso,"all");
     
     MyhistoManager.WriteMyHisto(PFElecPt,"all");
     MyhistoManager.WriteMyHisto(PFElecNeutralHadIso,"all");
     MyhistoManager.WriteMyHisto(PFElecChargedHadIso,"all");
     MyhistoManager.WriteMyHisto(PFElecPhotonIso,"all");
     MyhistoManager.WriteMyHisto(PFElecTrackIso,"all");
     MyhistoManager.WriteMyHisto(PFElecRelIso,"all");
     MyhistoManager.WriteMyHisto(PFElecEcalDriven,"all");
     MyhistoManager.WriteMyHisto(PFElecGsfEl,"all");
     MyhistoManager.WriteMyHisto(PFElecD0,"all");
     MyhistoManager.WriteMyHisto(PFElecId90,"all");
     MyhistoManager.WriteMyHisto(PFElecConv,"all");
     MyhistoManager.WriteMyHisto(PFElecRelIsoRhoCorrected,"all");
     MyhistoManager.WriteMyHisto(PFElecRelIsoNeutralRhoCorrected,"all");
     
     MyhistoManager.WriteMyHisto(PFMuonPt,"all");
     MyhistoManager.WriteMyHisto(PFMuonNeutralHadIso,"all");
     MyhistoManager.WriteMyHisto(PFMuonChargedHadIso,"all");
     MyhistoManager.WriteMyHisto(PFMuonPhotonIso,"all");
     MyhistoManager.WriteMyHisto(PFMuonTrackIso,"all");
     MyhistoManager.WriteMyHisto(PFMuonRelIso,"all");
     MyhistoManager.WriteMyHisto(PFMuonType,"all");
     MyhistoManager.WriteMyHisto(PFMuonChi2,"all");
     MyhistoManager.WriteMyHisto(PFMuonTrValHit,"all");
     MyhistoManager.WriteMyHisto(PFMuonValHit,"all");
     MyhistoManager.WriteMyHisto(PFMuonD0,"all");
     MyhistoManager.WriteMyHisto(PFMuonZvx,"all");
     MyhistoManager.WriteMyHisto(PFMuonRelIsoRhoCorrected,"all");
     MyhistoManager.WriteMyHisto(PFMuonRelIsoNeutralRhoCorrected,"all");

//      MyhistoManager.WriteMyHisto(PATElecPt_FromW,"all");
//      MyhistoManager.WriteMyHisto(PATElecRelIso_FromW,"all");
//      MyhistoManager.WriteMyHisto(PATMuonPt_FromW,"all");
//      MyhistoManager.WriteMyHisto(PATMuonRelIso_FromW,"all");
//      MyhistoManager.WriteMyHisto(PATElecPt_FromW_Cut015,"all");
//      MyhistoManager.WriteMyHisto(PATMuonPt_FromW_Cut015,"all");
     
     MyhistoManager.WriteMyHisto(PFElecPt_FromW,"all");
     MyhistoManager.WriteMyHisto(PFElecNeutralHadIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFElecChargedHadIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFElecPhotonIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFElecTrackIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFElecRelIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFMuonPt_FromW,"all");
     MyhistoManager.WriteMyHisto(PFMuonNeutralHadIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFMuonChargedHadIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFMuonPhotonIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFMuonTrackIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFMuonRelIso_FromW,"all");
     MyhistoManager.WriteMyHisto(PFElecPt_FromW_Cut015,"all");
     MyhistoManager.WriteMyHisto(PFMuonPt_FromW_Cut015,"all");
     MyhistoManager.WriteMyHisto(PFMuonType_FromW,"all");
     
//      MyhistoManager.WriteMyHisto(PATElecPt_FromBC,"all");
//      MyhistoManager.WriteMyHisto(PATElecRelIso_FromBC,"all");
//      MyhistoManager.WriteMyHisto(PATMuonPt_FromBC,"all");
//      MyhistoManager.WriteMyHisto(PATMuonRelIso_FromBC,"all");
//      MyhistoManager.WriteMyHisto(PATElecPt_FromBC_Cut015,"all");
//      MyhistoManager.WriteMyHisto(PATMuonPt_FromBC_Cut015,"all");
     
     MyhistoManager.WriteMyHisto(PFElecPt_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFElecNeutralHadIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFElecChargedHadIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFElecPhotonIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFElecTrackIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFElecRelIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFMuonPt_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFMuonNeutralHadIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFMuonChargedHadIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFMuonPhotonIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFMuonTrackIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFMuonRelIso_FromBC,"all");
     MyhistoManager.WriteMyHisto(PFElecPt_FromBC_Cut015,"all");
     MyhistoManager.WriteMyHisto(PFMuonPt_FromBC_Cut015,"all");
     MyhistoManager.WriteMyHisto(PFMuonType_FromBC,"all");
     
//      PFElecRelIsoEff_FromW_TTbarSig.Write();
//      PFElecRelIsoEff_FromBC_TTbarBkg.Write();
//      PFMuonRelIsoEff_FromW_TTbarSig.Write();
//      PFMuonRelIsoEff_FromBC_TTbarBkg.Write();
     
//      PATElecRelIsoEff_FromW_TTbarSig.Write();
//      PATElecRelIsoEff_FromBC_TTbarBkg.Write();
//      PATMuonRelIsoEff_FromW_TTbarSig.Write();
//      PATMuonRelIsoEff_FromBC_TTbarBkg.Write();
     
//      PFElecRelIsoPerf->Write();
//      PFMuonRelIsoEff_FromW_TTbarSig.Write();
//      PFMuonRelIsoEff_FromBC_TTbarBkg.Write();
//      PFMuonRelIsoPerf->Write();
     
//      PATElecRelIsoPerf->Write();
//      PATMuonRelIsoPerf->Write();
     
  fout->Close();
  
  delete fout;
  
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<"    End of the program   "<<endl;
    cout<<"#########################"<<endl;
  }
  
  return (0);
  
  
}
// void EffCompute(TH1F& h1,TH1F& h3,TH1F& h2,TH1F& h4, TGraph& g){
 void EffCompute(TH1F& h1,TH1F& h3,TH1F& h2,TH1F& h4, float X[], float Y[]){


     float num1=0.;
     float ntot1 = 0;
     for (int i=0; i<h1.GetNbinsX(); i++) {
        ntot1 += h1.GetBinContent(i+1);
     }
     for (int i=0; i<h1.GetNbinsX(); i++) {
	num1 += h1.GetBinContent(i+1);
	float n1 = num1/ntot1;
	h3.SetBinContent(i+1,n1);
    }

      float num2=0.;
      float ntot2 = 0;
      for (int i=0; i<h2.GetNbinsX(); i++) {
	 ntot2 += h2.GetBinContent(i+1);
      }
      for (int i=0; i<h2.GetNbinsX(); i++) {
	 num2 += h2.GetBinContent(i+1);
	 float n2 = num2/ntot2;
	 h4.SetBinContent(i+1,n2);
      }

   const int maxbin=200;
//    float X[maxbin];
//    float Y[maxbin];
   
   for (int i=0; i<h1.GetNbinsX(); i++) {
        X[i] = h3.GetBinContent(i);
//	std::cout<<"X "<<X[i]<<std::endl; 
   }
   
   for (int i=0; i<h2.GetNbinsX(); i++) {
        Y[i] = h4.GetBinContent(i);
//	std::cout<<"Y "<<Y[i]<<std::endl; 
   }
   
   TGraph* g1 = new TGraph(h1.GetNbinsX(), X, Y);
// //   TGraph g1(h1.GetNbinsX(), X, Y);
//    g = &(g1->Clone());

}
  
  
