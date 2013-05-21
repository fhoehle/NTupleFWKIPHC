#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/BtagSFHistoManager.h"
#include "../../Plots/interface/JetHistoManager.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

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
  string xmlFileName = string ("../../config/forLike.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );

//--------------------------
  //param de syst
  //Jet Energy Scale JES
  Syst SysJES;
  //Jet Energy Resolution JES
  Syst SysJER;
  //Unclustered Missing Energy Scale METS
  Syst SysMETS;
  //Electron Energy Scale EES
  Syst SysEES;
  //Muon Energy Scale MES
  Syst SysMES;

  anaEL.LoadSystematics (string ("JES"), SysJES);
  anaEL.LoadSystematics (string ("JER"), SysJER);
  anaEL.LoadSystematics (string ("METS"), SysMETS);
  anaEL.LoadSystematics (string ("EES"), SysEES);
  anaEL.LoadSystematics (string ("MES"), SysMES);

  bool applyJES= SysJES.doIt;
  float JESParam=SysJES.mean; // SysJES.m1sigma or SysJES.p1sigma
  bool applyJER= SysJER.doIt;
  float JERParam=SysJER.mean; // SysJER.m1sigma or SysJER.p1sigma 
  bool applyMET= SysMETS.doIt;
  float METScale=SysMETS.mean;  // SysMETS.m1sigma or SysMETS.p1sigma
  bool applyMES= SysMES.doIt;
  float MESParam=SysMES.mean;  // SysMES.m1sigma or SysMES.p1sigma
  bool applyEES= SysEES.doIt;
  float EESParam=SysEES.mean;  // SysEES.m1sigma or SysEES.p1sigma
  cout << " applyJES " << applyJES << " " << JESParam << endl; 
  cout << " applyJER " << applyJER << " " << JERParam << endl; 
  cout << " applyMES " << applyMES << " " << MESParam << endl; 
  cout << " applyEES " << applyEES << " " << EESParam << endl; 
//--------------------------


  TopTree::NTEvent * event = 0;

  //Book keeping of standard histos
  

//1. Btag 

  vector<string> channelList;
  channelList.push_back(string ("di-lept"));
  vector<string> cutList;
  cutList.push_back(string ("NJets=1"));
  cutList.push_back(string ("NJets=2"));
  cutList.push_back(string ("NJets>=3"));
  cutList.push_back(string ("NJets>=2"));

  BtagSFHistoManager histoManager;
  histoManager.LoadDatasets(datasets);    
  histoManager.LoadSelectionSteps(cutList);
  histoManager.LoadChannels(channelList);
//  histoManager.LoadSelectionSteps(sel.GetCutList());
//  histoManager.LoadChannels(sel.GetChannelList());

/*
  const int nb=7;
  float valsfb[nb]={0.8, 0.9, 0.95, 1., 1.05, 1.1, 1.2};
  const int nl=5;
  float valsfl[nl]={0.75, 0.9, 1., 1.1, 1.25};
*/
  const int nb=3;
  float valsfb[nb]={0.9,1.,1.1};
  const int nl=3;
  float valsfl[nl]={0.75, 1.,1.25};
  histoManager.CreateHistos(nb, valsfb,  nl, valsfl);	
  //////////////////////
 
//2. Njet 

  vector<string> cutList2;
  cutList2.push_back(string ("Selection"));
  JetHistoManager jetHistos;
  jetHistos.LoadDatasets(datasets);
  jetHistos.LoadSelectionSteps(cutList2);
  jetHistos.LoadChannels(channelList);
  jetHistos.CreateHistos();


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
    cout << "NEvents = " << datasets[d].NofEvtsToRunOver() << endl;
    
  
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < datasets[d].NofEvtsToRunOver(); ievt++) {

      float weight = 1.;
      if(datasets[d].isData() == false) weight = datasets[d].NormFactor()*Luminosity; //if Data , weight = 1

      vector<float> weightb;
      weightb.push_back(weight);  // weight of the event
      weightb.push_back(0.);      // Proba of 0 jet
      weightb.push_back(0.);      // Proba of 1 jet;
      weightb.push_back(0.);      // Proba of 2 jets;
      weightb.push_back(0.);      // Proba of at least 3 jets;


      datasets[d].eventTree ()->GetEntry (ievt);
     
      if(ievt%1000 == 0) cout << "number of processed events " << ievt << endl;

     //cout << "event number=" << event->eventNb << endl;
     //Load event for the selection
     sel.LoadEvent(event);

//--------------------------
      //Manage DY samples to avoid overlaps
      double dileptInvMass = 0;
      if( (event->zAndDecays).size() > 0){
        TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
        dileptInvMass = dilept.M();
      }

      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50 ) cout << "problem !!!" << endl;
      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50) continue;

      if(datasets[d].Name()=="DYToMuMu_M-20"       && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      //if(datasets[d].Name()=="DYToEE_M-20"       && (dileptInvMass < 50 && dileptInvMass > 20) ) cout << "is contributing " << endl;;
      if(datasets[d].Name()=="DYToEE_M-20"         && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToTauTau_M-20"     && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToEE_M-10To20"     &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToTauTau_M-10To20" &&  dileptInvMass > 20) continue;

      bool dysample=false;
      if (datasets[d].Name()=="Zjets" || datasets[d].Name()=="DYToMuMu_M-20" || datasets[d].Name()=="DYToEE_M-20" || datasets[d].Name()=="DYToTauTau_M-20"
          || datasets[d].Name()=="DYToMuMu_M-10To20"  || datasets[d].Name()=="DYToEE_M-10To20" || datasets[d].Name()=="DYToTauTau_M-10To20") dysample=true;
//--------------------------





//--------------------------
     //Collection of selected objects
     vector<NTVertex>   selVertices  = sel.GetSelectedVertex();

     vector<NTElectron> selElectrons = sel.GetSelectedElectrons(applyEES, EESParam); 
     vector<NTMuon>     selMuons     = sel.GetSelectedMuons(applyMES, MESParam);
     string CandType; // ee - emu - mumum or false
     vector<NTElectron> candElec;
     vector<NTMuon> candMuon;
     sel.GetLeptonPair(selMuons, selElectrons, candMuon, candElec, CandType); // fill the variables

     vector<NTJet>      selJets      = sel.GetSelectedJets (candMuon, candElec, applyJES, JESParam, applyJER, JERParam);
//--------------------------


//-------------------------
     // selection
     int ok=1;
     // pair 
     if ( ! (CandType=="mumu"    || CandType=="ee"  || CandType=="emu"  ) ) ok=0;
     // trigger 
      if ( ok==1 && ! sel.passTriggerSelection ( &datasets[d], CandType)     ) ok=0;
     //  vertex 
      if ( ok==1 && ! selVertices.size()>0 )   ok=0;
     // M_ll
      float dimass = sel.DiLeptonMass (candMuon, candElec);
      if ( ok==1 && !dysample && ! sel.DiLeptonMassCut (candMuon, candElec, CandType) )  ok=0;
      if ( ok==1 && dysample && sel.DiLeptonMass(candMuon, candElec)<sel.GetMinValueMassCut() ) ok=0;
     // MET 
      if (CandType == string ("emu")) {
        if (sel.GetMET (applyJES, JESParam, applyMET, METScale).p4.Et () <= sel.GetMETCut().first) ok=0;
      }
      else if (CandType=="mumu"    || CandType=="ee") {
        if (sel.GetMET (applyJES, JESParam, applyMET, METScale).p4.Et () <= sel.GetMETCut().second) ok=0;
      }
      // NJets
      int iNjetSel=-1;
      if (selJets.size()==1) iNjetSel=0;
      if (selJets.size()==2) iNjetSel=1;
      if (selJets.size()>=3) iNjetSel=2;

      if (ok==1) {
        jetHistos.Fill(selJets, 0, 0, d, weight);
      }

      if (ok==1 && iNjetSel<0) ok=0;

     if (ok==1) {
//       if (ievt<15) cout << " ievt " << ievt << " sel ok " << iNjetSel;
       // weight btag
       //  boucle sur sfl et sfb
       if (sel.GetFlagb() == 1) {
        for (int sfb_index=0; sfb_index<nb; sfb_index++) {
         for (int sfl_index=0; sfl_index<nl; sfl_index++) {
          vector< float > weight_temp = sel.GetSFBweight().GetWeigth4BSel (sel.GetMethodb(), sel.GetSystb(), selJets, valsfb[sfb_index],valsfl[sfl_index]);
          // do not change weightb[0] ; //weight of the event
          weightb[1] = weightb[0]*weight_temp[1];  // weight * proba 0 jet
          weightb[2] = weightb[0]*weight_temp[2];  // weight * proba 1 jet
          weightb[3] = weightb[0]*weight_temp[3];  // weight * proba 2 jets
          weightb[4] = weightb[0]*weight_temp[4];  // weight * proba at least 3 jets
//          if (ievt<15) cout << " sf1 " << valsfb[sfb_index] << " sf2 " << valsfl[sfl_index] << " w " << weightb[0] 
//               << " sel0 " << weightb[1] << " sel1 " << weightb[2] << " sel2 " << weightb[3]   
//               << " sel3 " << weightb[4] << endl;
          histoManager.Fill(sfb_index, sfl_index, 0, iNjetSel,  d, weightb);
          if (iNjetSel>=1 ) {
             // fill also the case NJets>=2 
             histoManager.Fill(sfb_index, sfl_index, 0, 3,  d, weightb);
          }
         } // sfl - end loop
        } // seb - end loop
       } // flag cut
      } // ok cut
//---------------------------
      

    
     
    }  // end of loop over evts


  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 


 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////


  histoManager.MergeMCDatasets2D();
  histoManager.Compute();
  histoManager.PlotsCutByCut2D();
  jetHistos.MergeMCDatasets();
  
  ///////////////

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Write output root file "<<endl;
	cout<<"#########################"<<endl;
  }
  TFile* fout  = new TFile("DistriForLikelihood2.root","RECREATE");

  if (sel.GetFlagb() == 1) {
   TDirectory * dir = 0;
   dir = fout->mkdir("SFmap");
   histoManager.Write2D(dir);


   TDirectory* dir1 = dir;
   dir1 = dir->mkdir("TGraphEff");
   histoManager.PlotTGraph(dir1);


   TDirectory * dir2 = 0;
   dir2 = fout->mkdir("NJetMult");
   jetHistos.Write(dir2);

   dir2 =0;
   delete dir2;
   dir1 =0;
   delete dir1;
   dir =0;
   delete dir;
  }

  fout->Close();
  
  //Clear histos before deleting the TFile
  histoManager.Clear();  
  jetHistos.Clear();

  delete fout;

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<"    End of the program   "<<endl;
	cout<<"#########################"<<endl;
  }

  return (0);
}
