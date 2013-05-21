#include <iomanip>
#include <iostream>
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
  TFile* fout  = new TFile("MyDYEstimate.root","RECREATE");

  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/MyDYEstimate.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  TopTree::NTEvent * event = 0;
  //Selection table
  // 4 tables: ee - emu - mumu - allChannels
  SelectionTable selTable_allChannels(sel.GetCutList(),datasets, string("*"));
  SelectionTable selTable_ee(  sel.GetCutList(),datasets, string("ee")  );
  SelectionTable selTable_emu( sel.GetCutList(),datasets, string("emu") );
  SelectionTable selTable_mumu(sel.GetCutList(),datasets, string("mumu"));

 
  vector<string> CutName;
  
// Here define the studied channel (ee/mumu)
  //string ChannelName  = "ee";
  string ChannelName  = "mumu";
//  
  CutName.push_back("Total");
  CutName.push_back("Trigger");
  CutName.push_back("Dilepton pair and VxP"); 
  if ( ChannelName != "emu" )  CutName.push_back("M($ll$) $\\notin$ [76,106] GeV$/c^2$");
  CutName.push_back("$E_T(jet1,2)$ $>$ 30 GeV");
  if ( ChannelName != "emu" ) {CutName.push_back("$\\not\\!\\!E_T$ $>$ 30 GeV");} else {CutName.push_back("$\\not\\!\\!E_T$ $>$ 20 GeV");} 
  CutName.push_back("btag ($\\ge$1)"); 
  CutName.push_back("btag ($\\ge$2)"); 
  int ITypeMC	  = -1;
  int ICut	  = -1;  
  int IChannel    = -1;  
  if (  ChannelName  == "mumu" )  IChannel = 0;
  if (  ChannelName  == "ee" )    IChannel = 1;
  if (  ChannelName  == "emu" )   IChannel = 2;
  
  float Dweight[101]; 
  for (unsigned int ii = 0; ii < 101; ii++) { Dweight[ii]=0;}
  

  //Background estimation methods
  DYEstimation DYest;
  //the following objects has to be internalized in the class itself
  std::vector<TH1F*> hlistemu;
  std::vector<TH1F*> hlistSR;
  std::vector<TH1F*> hlistCR;
  
  int nbins = 200;
  float minx = 0.;
  float maxx = 350;
  
  TH1F* halllistemu = new TH1F("halllistemu","halllistemu",nbins,minx,maxx);
  TH1F* hemudata    = new TH1F("hemudata","hemudata",nbins,minx,maxx);
  TH1F* halllistSR  = new TH1F("halllistSR","halllistSR",nbins,minx,maxx);
  TH1F* halllistCR  = new TH1F("halllistCR","halllistCR",nbins,minx,maxx);
  TH1F* hDataSR     = new TH1F("hDataSR","hDataSR",nbins,minx,maxx);
  TH1F* hDataCR     = new TH1F("hDataCR","hDataCR",nbins,minx,maxx);
  
  TH1F* NjetsDataCR     = new TH1F("NjetsDataCR","NjetsDataCR",5,-0.5,4.5);
  TH1F* NjetsDataSR     = new TH1F("NjetsDataSR","NjetsDataSR",5,-0.5,4.5);
  TH1F* NjetsMCCR     = new TH1F("NjetsMCCR","NjetsMCCR",5,-0.5,4.5);
  TH1F* NjetsMCSR     = new TH1F("NjetsMCSR","NjetsMCSR",5,-0.5,4.5);
  //////////////////////

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
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Loop over the datasets  "<<endl;
	cout<<"#########################"<<endl;
  }

// Creation of histogram list
  for (unsigned int d = 0; d < datasets.size (); d++) {
    //temporary
    hlistemu  = DYest.CreateDYHisto("emu",datasets[d].Name(),nbins,minx,maxx);
    hlistSR   = DYest.CreateDYHisto("SR",datasets[d].Name(),nbins,minx,maxx);
    hlistCR   = DYest.CreateDYHisto("CR",datasets[d].Name(),nbins,minx,maxx);
  }
  bool FoundWjets = false;
   std::cout<<"hlistSR.size() "<<hlistSR.size() <<std::endl;
   for (unsigned int j=0; j < hlistSR.size(); j++) {
   std::cout<<"hlistSR[j]->GetName() "<<hlistSR[j]->GetName()<<std::endl;
    if ( hlistSR[j]->GetName()==string("WjetsSR") ) FoundWjets = true;  
   }
   if ( !FoundWjets ) { 
      hlistSR   = DYest.CreateDYHisto("SR","Wjets",nbins,minx,maxx);
      hlistCR   = DYest.CreateDYHisto("CR","Wjets",nbins,minx,maxx);
      hlistemu   = DYest.CreateDYHisto("emu","Wjets",nbins,minx,maxx);
   }  
  bool FoundVV = false;
   for (unsigned int j=0; j < hlistSR.size(); j++) {
      if ( hlistSR[j]->GetName()==string("VVSR") ) FoundVV = true;  
   }
   if ( !FoundVV ) { 
      hlistSR   = DYest.CreateDYHisto("SR","VV",nbins,minx,maxx);
      hlistCR   = DYest.CreateDYHisto("CR","VV",nbins,minx,maxx);
      hlistemu   = DYest.CreateDYHisto("emu","VV",nbins,minx,maxx);
   }  
  
  
  for (unsigned int d = 0; d < datasets.size (); d++) {

    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    
//     //temporary
//     hlistemu  = DYest.CreateDYHisto("emu",datasets[d].Name(),nbins,minx,maxx);
//     hlistSR   = DYest.CreateDYHisto("SR",datasets[d].Name(),nbins,minx,maxx);
//     hlistCR   = DYest.CreateDYHisto("CR",datasets[d].Name(),nbins,minx,maxx);
    

    //////////////////////
    //LOOP OVER THE EVENTS
    //////////////////////
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      
      
      
      if(ievt%10000 == 0) cout << " number of events " << ievt << endl;;
      
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
     
     //Load event for the selection
     sel.LoadEvent(event);

     //Collection of selected objects
     vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
     vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
     vector<NTMuon>     selMuons     = sel.GetSelectedMuons();
     vector<NTJet>      selJets      = sel.GetSelectedJets();
     NTMET met                       = sel.GetMET(); 
 
     //Candidate pair of lepton
     string CandType; // ee - emu - mumum or false
     vector<NTElectron> candElec;
     vector<NTMuon> candMuon;


     bool IsSignal = false;
     int LastStep = 0;
     bool IsData    = false;
     bool IsDataEMU = false;
     
     
     
     
      
     //Manage DY samples to avoid overlaps
     double dileptInvMass = 0;
     if( (event->zAndDecays).size() > 0){
       TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
       dileptInvMass = dilept.M();
     }
     
     if(datasets[d].Name()=="Zjets" && dileptInvMass < 50 ) cout << "problem !!!" << endl;
      
     if(datasets[d].Name()=="Zjets" && dileptInvMass < 50) continue;
     
     if(datasets[d].Name()=="DYToMuMu_M-20"	  && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
     //if(datasets[d].Name()=="DYToEE_M-20"	  && (dileptInvMass < 50 && dileptInvMass > 20) ) cout << "is contributing " << endl;;
     if(datasets[d].Name()=="DYToEE_M-20"	  && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
     if(datasets[d].Name()=="DYToTauTau_M-20"	  && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
     if(datasets[d].Name()=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20) continue;
     if(datasets[d].Name()=="DYToEE_M-10To20"	  &&  dileptInvMass > 20) continue;
     if(datasets[d].Name()=="DYToTauTau_M-10To20" &&  dileptInvMass > 20) continue;
      
     
     
     
     
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
        if (IsSignal ) { ITypeMC = 0;} else { ITypeMC = 1;}
     }
// modifdg     
//      if ( datasets[d].Name()=="Zjets"   || 
// 	   datasets[d].Name()=="DYToMuMu_M-20"      || 
// 	   datasets[d].Name()=="DYToEE_M-20"        || 
// 	   datasets[d].Name()=="DYToTauTau_M-20"    || 
// 	   datasets[d].Name()=="DYToMuMu_M-10To20"  || 
// 	   datasets[d].Name()=="DYToEE_M-10To20"    || 
// 	   datasets[d].Name()=="DYToTauTau_M-10To20"
// 	   ) { 
//         ITypeMC = 2; IsSignal = false;
//      }
// endmodifdg     
     if ( datasets[d].Name()=="DYToMuMu_M-20"    ) { 
        ITypeMC = 21; IsSignal = false;
     }
     if ( datasets[d].Name()=="DYToEE_M-20"    ) { 
        ITypeMC = 22; IsSignal = false;
     }
      if ( datasets[d].Name()=="DYToTauTau_M-20"    ) { 
        ITypeMC = 23; IsSignal = false;
     }
     if ( datasets[d].Name()=="DYToMuMu_M-10To20"    ) { 
        ITypeMC = 24; IsSignal = false;
     }
     if ( datasets[d].Name()=="DYToEE_M-10To20"    ) { 
        ITypeMC = 25; IsSignal = false;
     }
     if ( datasets[d].Name()=="DYToTauTau_M-10To20"    ) { 
        ITypeMC = 26; IsSignal = false;
     }
    if ( datasets[d].Name()=="Zjets" ) { 
        ITypeMC = 2; IsSignal = false;
     }
// modifdg     
//      if ( datasets[d].Name()=="WjetsMu" ||
//           datasets[d].Name()=="WjetsE"  ||
//           datasets[d].Name()=="WjetsTau") { 
//         ITypeMC = 3; IsSignal = false;
//      }
     if ( datasets[d].Name()=="WjetsMu" ) { 
        ITypeMC = 31; IsSignal = false;
     }	
     if ( datasets[d].Name()=="WjetsE" ) { 
        ITypeMC = 32; IsSignal = false;
     }	
     if ( datasets[d].Name()=="WjetsTau" ) { 
        ITypeMC = 33; IsSignal = false;
     }
// endmodifdg     
     if ( datasets[d].Name()=="Wjets" ) { 
        ITypeMC = 3; IsSignal = false;
     }
     if ( datasets[d].Name()=="SingleToptW" ) { 
        ITypeMC = 4; IsSignal = false;
     }
// modifdg     
//      if ( datasets[d].Name()=="WZ" || datasets[d].Name()=="WW" || datasets[d].Name()=="ZZ"  ) { 
//         ITypeMC = 5; IsSignal = false;
//      }
     if ( datasets[d].Name()=="WW"  ) { 
        ITypeMC = 51; IsSignal = false;
     }
     if ( datasets[d].Name()=="WZ"  ) { 
        ITypeMC = 52; IsSignal = false;
     }
     if ( datasets[d].Name()=="ZZ"  ) { 
        ITypeMC = 53; IsSignal = false;
     }
// endmodifdg     
     if ( datasets[d].Name()=="VV" ) { 
        ITypeMC = 5; IsSignal = false;
     }
     if ( datasets[d].Name()=="DYee" ) { 
        ITypeMC = 6; IsSignal = false; 
     }
     if ( datasets[d].Name()=="DYmumu" ) { 
        ITypeMC = 7; IsSignal = false;
     }
// modifdg     
     if ( datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMu" || datasets[d].Name()=="Data" || datasets[d].Name()=="DATA") { 
// endmodifdg     
        ITypeMC = 100; IsData = true; 
     }
     if ( datasets[d].Name()=="DataEMU" || datasets[d].Name()=="DATAEMU") { 
       ITypeMC = 99; IsDataEMU = true; 
     }
     
// modifdg     
//     IsDataEMU = IsData;
// endmodifdg     

     //Dweight[ITypeMC] = Luminosity*datasets[d].Xsection()/datasets[d].NofEvtsToRunOver();
      Dweight[ITypeMC] = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
//     std::cout<<"datasets[d].getNSkimmedEvent() "<<datasets[d].getNSkimmedEvent()<<std::endl;
      if ( ievt==0 ) {
        std::cout<<"ITypeMC,Dweight,Xsection "<< ITypeMC<<" "<<Dweight[ITypeMC]<<" "<<datasets[d].Xsection()<<std::endl;
        std::cout<<"datasets[d].getNSkimmedEvent() "<<datasets[d].getNSkimmedEvent()<<std::endl;
      }
     
// modifdg     
     if ( datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMu" || datasets[d].Name()=="Data" || datasets[d].Name()=="DATA") Dweight[ITypeMC] = 1;
// endmodifdg     

     bool TriggerPassed_mumu_MC = (event->TriggerPassed("HLT_Mu9"));
     
// HLT_Ele10_SW_L1R or HLT_Ele10_LW_L1R to be verified
     bool TriggerPassed_ee_MC   = (event->TriggerPassed("HLT_Ele10_SW_L1R") && (sel.eleHLTMatch>15) );
     bool TriggerPassed_mumu_DATA = false;
     bool TriggerPassed_ee_DATA   = false;
     
      
      if ( ( event->runNb<138000  && event->TriggerPassed("HLT_Ele10_LW_L1R") && (sel.eleHLTMatch>15) ) ||
	   ( event->runNb>=141900 && event->runNb<144000 && event->TriggerPassed("HLT_Ele15_SW_L1R") ) ||
	   ( event->runNb>144000  && event->runNb<=144114 && (event->TriggerPassed("HLT_Ele15_SW_CaloEleId_L1R")||event->TriggerPassed("HLT_Ele20_SW_L1R")||event->TriggerPassed("HLT_DoubleEle10_SW_L1R") ) ) ||
	   ( event->runNb>146000  && event->runNb<147120 && (event->TriggerPassed("HLT_DoubleEle10_SW_L1R")||event->TriggerPassed("HLT_Ele17_SW_CaloEleId_L1R") ) ) ||
	   ( event->runNb>147120  && event->runNb<148100 && (event->TriggerPassed("HLT_DoubleEle15_SW_L1R_v1")||event->TriggerPassed("HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1")||event->TriggerPassed("HLT_Ele17_SW_TightEleId_L1R") ) ) ||
	   ( event->runNb>148100  && (event->TriggerPassed("HLT_DoubleEle17_SW_L1R_v1")||
				     event->TriggerPassed("HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2")|| 
				     event->TriggerPassed("HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1")|| 
				     event->TriggerPassed("HLT_Ele22_SW_TighterEleId_L1R_v3")|| 
				     event->TriggerPassed("HLT_Ele22_SW_TighterEleId_L1R_v2")|| 
				     event->TriggerPassed("HLT_Ele17_SW_TighterEleIdIsol_L1R_v3")|| 
				     event->TriggerPassed("HLT_Ele17_SW_TighterEleIdIsol_L1R_v2")  
				     ) ) ) TriggerPassed_ee_DATA   = true;
      
       if ( ( event->runNb<145000 && event->TriggerPassed("HLT_Mu9")  ) ||  
	   ( event->runNb>145000 && event->runNb<147120 && event->TriggerPassed("HLT_Mu11")  ) || 
	   ( event->runNb>147120 && event->TriggerPassed("HLT_Mu15_v1")  ) ) TriggerPassed_mumu_DATA = true;
      
     
     bool TriggerPassed_emu_MC   = (TriggerPassed_mumu_MC || TriggerPassed_ee_MC);
     bool TriggerPassed_emu_DATA   = //(TriggerPassed_mumu_DATA || TriggerPassed_ee_DATA);
          (TriggerPassed_ee_DATA && datasets[d].Name()=="DataEG")
	||( TriggerPassed_mumu_DATA && datasets[d].Name()=="DataMu" && !TriggerPassed_ee_DATA);

// Passage trigger emu
     if ( ( (!IsDataEMU && !IsData && TriggerPassed_emu_MC)||(IsDataEMU && TriggerPassed_emu_DATA) )  ){
     
  //cout << " pass trigger sel emu" << endl;
         sel.GetLeptonPair(candMuon, candElec, CandType ); 
	 
// Passage dilepton candidat
         if ( CandType=="emu"  && selVertices.size()>0 ) {
           vector<NTJet>      theselJets = sel.GetSelectedJets(candMuon, candElec);
	   
  //cout << " found emu cand " << endl;
// Passage multiplicite de jets
           if ( theselJets.size()>=2 )  {
	       
               DYest.FillDYHistos("emu",hlistemu,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
	   
// Passage met
               if (  met.p4.Et()>sel.GetMETCut().first )  {
		 int NBtaggedJets = 0;
		 

// Passage Btagging
		 int AlgoBtag = sel.GetbtagAlgo();
		 for(unsigned int j=0;j<theselJets.size();j++){
		   if ( AlgoBtag==0 &&  theselJets[j].TCDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
		   if ( AlgoBtag==1 &&  theselJets[j].SVDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
		   if ( AlgoBtag==2 &&  theselJets[j].SMDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
		 }			
                 if ( NBtaggedJets>=1 )  {
	         }//n btagged jet==1
                 if ( NBtaggedJets>=2 )  {
	         }//n btagged jet==2
		 
	       }//met.p4.Et()>30 )
	     
	   }//theselJets.size()>=2
	   
	 } //CandType (Dilepton pair+VxP)
	 
     }//event->TriggerPassed()	 
    


// Passage trigger pour ee OR mumu
//      if (  (ChannelName=="mumu" && TriggerPassed_mumu_MC)  || 
//            (ChannelName=="ee" && TriggerPassed_ee_MC)  ) {
     if (  (ChannelName=="mumu" && ((!IsData && TriggerPassed_mumu_MC) || (IsData && TriggerPassed_mumu_DATA)) ) || 
           (ChannelName=="ee" && ((!IsData && TriggerPassed_ee_MC) || (IsData && TriggerPassed_ee_DATA)))  ) {
     
         sel.GetLeptonPair(candMuon, candElec, CandType ); 
	 
// Passage dilepton candidat
         if ( ((CandType=="mumu" && ChannelName=="mumu") || 
	       (CandType=="ee" && ChannelName=="ee")    
	                                               ) && selVertices.size()>0 ) {
           TLorentzVector DiLepton_mumu;
           for(unsigned int i=0;i<candMuon.size();i++) DiLepton_mumu+=candMuon[i].p4;
           TLorentzVector DiLepton_ee;
           for(unsigned int i=0;i<candElec.size();i++) DiLepton_ee+=candElec[i].p4;
	   float InvDilMass = 0;
	   if (ChannelName=="mumu")  InvDilMass = DiLepton_mumu.M();
	   if (ChannelName=="ee")    InvDilMass = DiLepton_ee.M();
           vector<NTJet>      theselJets = sel.GetSelectedJets(candMuon, candElec);

// modifdg     
   if ( InvDilMass>20 ) {	   
// endmodifdg     

     //DYEstimation
           if ( CandType == ChannelName ){
              bool isinSR = false;
              bool isinCR = false;
	      
// cut au niveau dilepton pair ou apres la coupure en met
//              if ( theselJets.size()>1 ) {
              if ( theselJets.size()>1 &&  met.p4.Et()>sel.GetMETCut().second ) {

	         DYest.FillDYHistos("SR",hlistSR,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
		 isinSR = true;
	      }  
	      
// cut au niveau dilepton pair ou apres la coupure en met
//              if ( theselJets.size()<=1 ) { 
              if ( theselJets.size()<=1 &&  met.p4.Et()>sel.GetMETCut().second ) { 

	         DYest.FillDYHistos("CR",hlistCR,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
		 isinCR = true;
	      } 
	      
// cut au niveau dilepton pair ou apres la coupure en met
	      if ( met.p4.Et()>sel.GetMETCut().second ){  
                 DYest.FillNbofEventsInSRandCR(isinCR,isinSR,sel.DiLeptonMass(candMuon,candElec),datasets[d].Name(),Dweight[ITypeMC]);
                 if ( isinCR &&  IsData ) NjetsDataCR->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinSR &&  IsData ) NjetsDataSR->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinCR &&  !IsData ) NjetsMCCR->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinSR &&  !IsData ) NjetsMCSR->Fill(theselJets.size(),Dweight[ITypeMC]);
              }
	   }

// Passage invariante mass dilepton
           if ( InvDilMass<sel.GetZmassWindowCut().first || InvDilMass>sel.GetZmassWindowCut().second )  {

// Passage multiplicite de jets
             if ( theselJets.size()>=2 )  {
	       
// Passage met
               if (  met.p4.Et()>sel.GetMETCut().second )  {

		 int NBtaggedJets = 0;
// Passage Btagging
		 int AlgoBtag = sel.GetbtagAlgo();
		 for(unsigned int j=0;j<theselJets.size();j++){
		   if ( AlgoBtag==0 &&  theselJets[j].TCDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
		   if ( AlgoBtag==1 &&  theselJets[j].SVDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
		   if ( AlgoBtag==2 &&  theselJets[j].SMDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
		 }			
                 if ( NBtaggedJets>=1 )  {
	         }//n btagged jet==1
                 if ( NBtaggedJets>=2 )  {
	         }//n btagged jet==2
		 
	       }//met.p4.Et()>30 )
	     
	     }//theselJets.size()>=2
	   
	   }//DiLepton.M()<76 ||  DiLepton.M() >106
	     
 } // 20

	 } //CandType (Dilepton pair+VxP)
	 
     }//event->TriggerPassed()	 
     
     
 
    }  // end of loop over evts

  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 


 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

 
  //DYEstimation 
  DYest.MakeDYHistos("emu",hlistemu,halllistemu);
  DYest.MakeDYHistos("SR",hlistSR,halllistSR);
  DYest.MakeDYHistos("CR",hlistCR,halllistCR);
  DYest.MakeDataHistos("SR",hlistSR,hDataSR);
  DYest.MakeDataHistos("CR",hlistCR,hDataCR);
// modifdg
//  DYest.MakeDataHistos("emu",hlistemu,hemudata);
// endmodifdg
  
  
  
// Fit the emu channel to parametrize the shape of the non-DY events
  vector<double> parafit;
  parafit = DYest.MakeDYFits_emu(halllistemu);
// Fit the mumu or ee channel with the BW shape + parametrized shape of the non-DY events in the Signal Region for MC
// Take into account a scaled fraction of non-DY events for systematics
  std::cout<<"Appel a MakeDYFits_SR"<<std::endl;
  DYest.MakeDYFits_SR(halllistSR,nbins,minx,maxx,parafit,0,0,"MC",1.);
  
// Fit the mumu or ee channel with the BW shape in the Control Region for MC 
  std::cout<<"Appel a MakeDYFits_CR"<<std::endl;
  DYest.MakeDYFits_CR(halllistCR,nbins,minx,maxx,0,0,"MC");
  
  std::cout<<"appel a ComputeDYEstimatedEvents_MC"<<std::endl;
  DYest.ComputeDYEstimatedEvents_MC();
  std::cout<<"Number of MC DY events in Z peak (for CR) = "<<DYest.GetNDYMCinMz_CR()<<std::endl;
  std::cout<<"Number of MC DY events in Z peak (for SR) = "<<DYest.GetNDYMCinMz_SR()<<std::endl;
  std::cout<<"Number of MC DY events outside the Z peak (for CR) = "<<DYest.GetNDYMC_CR()<<std::endl;

// MC comparison between real and expected yields
  std::cout<<"Number of MC DY events outside the Z peak (for SR) = "<<DYest.GetNDYMC_SR()<<std::endl;
  std::cout<<"Number of ESTIMATED MC DY events outside the Z peak (for SR) = "<<DYest.GetNDYEstimatedMC() <<std::endl;

// Computation for Data
// Fit the emu channel to parametrize the shape of the non-DY events
  //parafit.clear();
  //parafit = DYest.MakeDYFits_emu(hemudata);
// Fit the mumu or ee channel with the BW shape + parametrized MC shape of the non-DY events in the Signal Region 
// Take into account a scaled fraction of non-DY events for systematics
  std::cout<<"Appel a MakeDYFits_SR pour Data"<<std::endl;
  DYest.MakeDYFits_SR(hDataSR,nbins,minx,maxx,parafit,0,0,"Data",1.);
  
// Fit the mumu or ee channel with the BW shape in the Control Region    
  std::cout<<"Appel a MakeDYFits_CR pour Data"<<std::endl;
  DYest.MakeDYFits_CR(hDataCR,nbins,minx,maxx,0,0,"Data");
  
// Compute the number of expected bckg according to 
// "Counting" method (simply count the number of events in each zones) or from a "Fit" method of the inv.mass distribution (effect of others process bckg for DY can be taken into account)
// "MC" or "DATA" origin for the scale factor (NofEventsintheMZpieak(inSR)/NofEventsintheMZpieak(inCR))
// modifdg
  DYest.ComputeDYEstimatedEvents_DATA("Counting","Data");
  std::cout<<"Counting Number of Data events outside the Z peak (for SR)                          = "<<DYest.GetNDATA_SR() <<std::endl;
  std::cout<<"Counting Number of ESTIMATED DY events outside the Z peak (for SR)                  =  "<<DYest.GetNDYEstimatedDATACounting() <<std::endl;
  std::cout<<"statistical error =  "<<DYest.GetNDYEstimatedStatErrorDATACounting() <<std::endl;
// endmodifdg
  DYest.ComputeDYEstimatedEvents_DATA("Fit","Data");
  std::cout<<"Fitting Number of Data events outside the Z peak (for SR)                          = "<<DYest.GetNDATA_SR() <<std::endl;
//  std::cout<<"Fitting Number of Data events outside the Z peak (for CR)                          = "<<DYest.GetNFittedDATA_CR() <<std::endl;
// better to count in the non Mz mass range for CR:
  std::cout<<"Fitting Number of Data events outside the Z peak (for CR)                          = "<<DYest.GetNDATA_CR() <<std::endl;
  std::cout<<"Fitting Number of ESTIMATED DY events outside the Z peak (for SR)                  = "<<DYest.GetNDYEstimatedDATA() <<std::endl;
  std::cout<<"statistical error =  "<<DYest.GetNDYEstimatedStatErrorDATA() <<std::endl;
  std::cout<<"to be compared with the number of MC DY events outside the Z peak (for SR) = "<< DYest.GetNDYMC_SR() <<std::endl;
  
//   TFile* fout  = new TFile("MyDYEstimate.root","RECREATE");
   fout->cd();
  
  halllistemu->Write(); 
  halllistSR->Write();  
  halllistCR->Write();  
  hDataSR->Write();     
  hDataCR->Write(); 
  hemudata->Write(); 
  NjetsDataCR->Write(); 
  NjetsDataSR->Write(); 
  NjetsMCCR->Write(); 
  NjetsMCSR->Write(); 
  /////////////////////
  
  //fout->Write();
  
  for(unsigned int i=0; i<hlistemu.size(); i++) hlistemu[i]->Write();
  for(unsigned int i=0; i<hlistSR.size();  i++) hlistSR[i]->Write();
  for(unsigned int i=0; i<hlistCR.size();  i++) hlistCR[i]->Write();
  
  
  fout->Write();
  fout->Close();

  delete fout;

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<"    End of the program   "<<endl;
	cout<<"#########################"<<endl;
  }

  return (0);
}
