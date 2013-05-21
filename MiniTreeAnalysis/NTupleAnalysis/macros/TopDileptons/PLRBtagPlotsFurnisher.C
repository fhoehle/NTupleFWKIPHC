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


//for b-tagging
void InitVectorOfWeight(vector<float>& weightb){
	    weightb.push_back (1.);
	    weightb.push_back (0.);
	    weightb.push_back (0.);
	    weightb.push_back (0.);
	    weightb.push_back (0.);
}

void LoadBWeight(Selection& sel, vector<float>& weightb, const vector<NTJet>& theselJets){
		vector < float >weight_temp = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJets);
		weightb[0] = weight_temp[0];	//weight of the event
		weightb[1] = weight_temp[1];	//proba 0 jet
		weightb[2] = weight_temp[2];	//proba 1 jet
		weightb[3] = weight_temp[3];	//proba 2 jets
		weightb[4] = weight_temp[4];	//proba at least 3 jets
}

using namespace TopTree;
using namespace std;

int main (int argc, char *argv[])
{
  cout << "#########################" << endl;
  cout << "Beginning of the program" << endl;
  cout << "#########################" << endl;

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
  if(argc<3){
	 cerr<<"usage: ./MyCutFlow_btagPlotsModif.C file.xml file.root"<<endl;
	 return(0);
  }
  string xmlFileName;
  cout<<"argc "<<argc<<" "<<argv[0]<<endl;
  if (argc>1 ) xmlFileName = string(argv[1]);
  else xmlFileName = string ("../../config/MyCutFlow_btagPlots.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets);	// now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel);	// now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo (DataType, Luminosity, verbosity);
  anaEL.LoadWeight (sel);	// now the parameters for SFBweight are initialized (for b-tag!)
  TopTree::NTEvent * event = 0;
  //Selection table
  // 4 tables: ee - emu - mumu - allChannels
  SelectionTable selTable_allChannels (sel.GetCutList (), datasets, string ("*"));
  SelectionTable selTable_ee (sel.GetCutList (), datasets, string ("ee"));
  SelectionTable selTable_emu (sel.GetCutList (), datasets, string ("emu"));
  SelectionTable selTable_mumu (sel.GetCutList (), datasets, string ("mumu"));

  //   //Book keeping of standard histos
  //   DiLepAnaHistoManager histoManager;
  //   histoManager.LoadDatasets(datasets);    
  //   histoManager.LoadSelectionSteps(sel.GetCutList());
  //   histoManager.LoadChannels(sel.GetChannelList());
  //   histoManager.CreateHistos();     
  //   //////////////////////

  //CutFlow tables
  double TabFlow1[5][101][101];	//central values [channel][typemc][cuttype]
  double TabFlow2[5][101][101];	// errors
  //dataset weights
  double Dweight[101];
  for (int k0 = 0; k0 < 5; ++k0) {
    for (int k1 = 0; k1 < 101; ++k1) {
      Dweight[k1] = 0.;
      for (int k2 = 0; k2 < 101; ++k2) {
	TabFlow1[k0][k1][k2] = 0.;
	TabFlow2[k0][k1][k2] = 0.;
      }
    }
  }
  vector < string > VecChannelName;
  vector < string > LoopChannelName;
  LoopChannelName.push_back ("mumu");
  LoopChannelName.push_back ("emu");
  LoopChannelName.push_back ("ee");
  bool IsSignal[3];
  IsSignal[0] = false;
  IsSignal[1] = false;
  IsSignal[2] = false;
   


// Here define the studied channel (ee/mumu/emu)
  string ChannelName = "emu";	// "mumu", "ee", "emu"

  bool IReweightWithVxP = true;
// Here define Scale Factors
  //SF_trigger applied for mumu
  double SF_trig_mu = 1.;
  //SF_trigger applied for emu
  double SF_trig_emu = 1.;	// 0.99;
  //SF_eID applied for ee
  double SF_e = 1.;		//0.965; //1;

  double SF_VxP1[100]= {0}; 
  double SF_VxP2[100]= {0}; 
  double SF_VxP3[100]= {0}; 
  if (!IReweightWithVxP ) {
     for (unsigned int k = 0; k < 100; k++) {
        SF_VxP1[k] = 1.;   
        SF_VxP2[k] = 1.;   
        SF_VxP3[k] = 1.;   
     }
  }
   //////////////////////
  //Load Vxp Reweighting
  //////////////////////
  if (IReweightWithVxP ) {
     TFile* fin1  = new TFile("../data/VxP_Weight_MuEG.root","READ");
     TH1F * R1 = new TH1F("R1","R1",20,-0.5,19.5);
     R1 = (TH1F*)fin1->Get("VxP_Reweight");
     for (unsigned int k = 1; k < R1->GetNbinsX()+1; k++) {
       SF_VxP1[k-1] = R1->GetBinContent(k);
       std::cout<<"#Bin,#Weight "<<k-1<<" "<<SF_VxP1[k-1]<<std::endl;
     }
     fin1->Close();
     TFile* fin2  = new TFile("../data/VxP_Weight_Mu.root","READ");
     TH1F * R2 = new TH1F("R2","R2",20,-0.5,19.5);
     R2 = (TH1F*)fin2->Get("VxP_Reweight");
     for (unsigned int k = 1; k < R2->GetNbinsX()+1; k++) {
       SF_VxP2[k-1] = R2->GetBinContent(k);
       std::cout<<"#Bin,#Weight "<<k-1<<" "<<SF_VxP2[k-1]<<std::endl;
     }
     fin2->Close();
     TFile* fin3  = new TFile("../data/VxP_Weight_EG.root","READ");
     TH1F * R3 = new TH1F("R3","R3",20,-0.5,19.5);
     R3 = (TH1F*)fin3->Get("VxP_Reweight");
     for (unsigned int k = 1; k < R3->GetNbinsX()+1; k++) {
       SF_VxP3[k-1] = R3->GetBinContent(k);
       std::cout<<"#Bin,#Weight "<<k-1<<" "<<SF_VxP3[k-1]<<std::endl;
     }
     fin3->Close();
  }
 

  cout << " ######################################" << endl;
  cout << "	Systematics		       " << endl;
  cout << " ######################################" << endl;
  //Jet Energy Scale JES
  Syst SysJES;
  //Jet Energy Resolution JER
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

  cout << " - JES: " << SysJES.doIt << " - " << SysJES.m1sigma << " - " << SysJES.mean << " - " << SysJES.p1sigma << endl;
  cout << " - JER: " << SysJER.m1sigma << " - " << SysJER.mean << " - " << SysJER.p1sigma << endl;
  cout << " - METS: " << SysMETS.m1sigma << " - " << SysMETS.mean << " - " << SysMETS.p1sigma << endl;
  cout << " - EES: " << SysEES.m1sigma << " - " << SysEES.mean << " - " << SysEES.p1sigma << endl;
  cout << " - MES: " << SysMES.doIt << " - " << SysMES.m1sigma << " - " << SysMES.mean << " - " << SysMES.p1sigma << endl;

  VecChannelName.push_back (ChannelName);
  vector < string > CutName;
  CutName.push_back ("Total");
  CutName.push_back ("Trigger");
  CutName.push_back ("Dilepton pair and VxP");
  CutName.push_back ("M($ll$) cut");
  CutName.push_back ("$E_T(jet1,2)$ $>$ 30 GeV");
  CutName.push_back ("$\\not\\!\\!E_T$ cut$");
  CutName.push_back ("btag ($\\ge$1)");
  CutName.push_back ("btag ($\\ge$2)");


  int ITypeMC = -1;
  int ICut = -1;
  int IChannel = -1;
  if (ChannelName == "mumu")
    IChannel = 0;
  if (ChannelName == "ee")
    IChannel = 1;
  if (ChannelName == "emu")
    IChannel = 2;

  //Scanning of SF_b
  const int nb_valsfb = 9;
  float valsfb[nb_valsfb] = { 0.8, 0.85, 0.9, 0.95, 1., 1.05, 1.1, 1.15, 1.2 };
  //Scanning of SF_l
  const int nb_valsfl = 11;
  float valsfl[nb_valsfl] = { 0.5, 0.6, 0.7, 0.8, 0.9, 1., 1.1, 1.2, 1.3, 1.4, 1.5 };


  //Book keeping of my histos
  HistoManager MyhistoManager;
  MyhistoManager.LoadDatasets (datasets);
  MyhistoManager.LoadSelectionSteps (CutName);
  MyhistoManager.LoadChannels (VecChannelName);
  MyhistoManager.AddHisto ("DilepMass", "DilepMass", "GeV", 80, 0., 200.);;
  //////////////////////

  //List of histograms
  std::vector < TH1F > MyHistos;
  std::vector < TH1F > MyHistos1;
  std::vector < TH1F > HInvM_ll_pair;
  std::vector < TH1F > HInvM_emu_pair;
  std::vector < TH1F > Hemu_Njets;
  std::vector < TH1F > Hemu_Njets_JES_minus;
  std::vector < TH1F > Hemu_Njets_JES_plus;
  std::vector < TH1F > Hemu_Njets_JER_minus;
  std::vector < TH1F > Hemu_Njets_JER_plus;
  std::vector < TH1F > Hemu_Njets_EES_minus;
  std::vector < TH1F > Hemu_Njets_EES_plus;
  std::vector < TH1F > Hemu_Njets_MES_minus;
  std::vector < TH1F > Hemu_Njets_MES_plus;
  std::vector < TH1F > HMet_emu;
  std::vector < TH1F > Hemu_NBjets;
  std::vector < TH1F > HCount_emu;
  std::vector < TH1F > HCount_emu_after1btag;
  std::vector < TH1F > Hemu_Njets_after1btag;
  //SFb
  std::vector < TH2D > HCount2D_emu_sfb_0b;
  std::vector < TH2D > HCount2D_emu_sfb_1b;
  std::vector < TH2D > HCount2D_emu_sfb_2b;
  std::vector < TH2D > HCount2D_emu_sfb_3b;
  std::vector < TH2D > HWeight2D_emu_sfb_0b;
  std::vector < TH2D > HWeight2D_emu_sfb_1b;
  std::vector < TH2D > HWeight2D_emu_sfb_2b;
  std::vector < TH2D > HWeight2D_emu_sfb_3b;
  //SFl
  std::vector < TH2D > HCount2D_emu_sfl_0b;
  std::vector < TH2D > HCount2D_emu_sfl_1b;
  std::vector < TH2D > HCount2D_emu_sfl_2b;
  std::vector < TH2D > HCount2D_emu_sfl_3b;
  std::vector < TH2D > HWeight2D_emu_sfl_0b;
  std::vector < TH2D > HWeight2D_emu_sfl_1b;
  std::vector < TH2D > HWeight2D_emu_sfl_2b;
  std::vector < TH2D > HWeight2D_emu_sfl_3b;
  //
  std::vector < TH2D > Hemu_NjetsNBjets;
  std::vector < TH2D > Hemu_NjetsNBjets_JES_minus;
  std::vector < TH2D > Hemu_NjetsNBjets_JES_plus;
  std::vector < TH2D > Hemu_NjetsNBjets_JER_minus;
  std::vector < TH2D > Hemu_NjetsNBjets_JER_plus;
  std::vector < TH2D > Hemu_NjetsNBjets_EES_minus;
  std::vector < TH2D > Hemu_NjetsNBjets_EES_plus;
  std::vector < TH2D > Hemu_NjetsNBjets_MES_minus;
  std::vector < TH2D > Hemu_NjetsNBjets_MES_plus;


  std::vector < TH1F > HInvM_mumu_pair;
  std::vector < TH1F > Hmumu_Njets;
  std::vector < TH1F > Hmumu_Njets_JES_minus;
  std::vector < TH1F > Hmumu_Njets_JES_plus;
  std::vector < TH1F > Hmumu_Njets_JER_minus;
  std::vector < TH1F > Hmumu_Njets_JER_plus;
  std::vector < TH1F > Hmumu_Njets_EES_minus;
  std::vector < TH1F > Hmumu_Njets_EES_plus;
  std::vector < TH1F > Hmumu_Njets_MES_minus;
  std::vector < TH1F > Hmumu_Njets_MES_plus;
  std::vector < TH1F > HMet_mumu;
  std::vector < TH1F > Hmumu_NBjets;
  std::vector < TH1F > HCount_mumu;
  std::vector < TH1F > HCount_mumu_after1btag;
  std::vector < TH1F > Hmumu_Njets_after1btag;
  //SFb
  std::vector < TH2D > HCount2D_mumu_sfb_0b;
  std::vector < TH2D > HCount2D_mumu_sfb_1b;
  std::vector < TH2D > HCount2D_mumu_sfb_2b;
  std::vector < TH2D > HCount2D_mumu_sfb_3b;
  std::vector < TH2D > HWeight2D_mumu_sfb_0b;
  std::vector < TH2D > HWeight2D_mumu_sfb_1b;
  std::vector < TH2D > HWeight2D_mumu_sfb_2b;
  std::vector < TH2D > HWeight2D_mumu_sfb_3b;
  //Sfl
  std::vector < TH2D > HCount2D_mumu_sfl_0b;
  std::vector < TH2D > HCount2D_mumu_sfl_1b;
  std::vector < TH2D > HCount2D_mumu_sfl_2b;
  std::vector < TH2D > HCount2D_mumu_sfl_3b;
  std::vector < TH2D > HWeight2D_mumu_sfl_0b;
  std::vector < TH2D > HWeight2D_mumu_sfl_1b;
  std::vector < TH2D > HWeight2D_mumu_sfl_2b;
  std::vector < TH2D > HWeight2D_mumu_sfl_3b;
  //
  std::vector < TH2D > Hmumu_NjetsNBjets;
  std::vector < TH2D > Hmumu_NjetsNBjets_JES_minus;
  std::vector < TH2D > Hmumu_NjetsNBjets_JES_plus;
  std::vector < TH2D > Hmumu_NjetsNBjets_JER_minus;
  std::vector < TH2D > Hmumu_NjetsNBjets_JER_plus;
  std::vector < TH2D > Hmumu_NjetsNBjets_EES_minus;
  std::vector < TH2D > Hmumu_NjetsNBjets_EES_plus;
  std::vector < TH2D > Hmumu_NjetsNBjets_MES_minus;
  std::vector < TH2D > Hmumu_NjetsNBjets_MES_plus;



  std::vector < TH1F > HInvM_ee_pair;
  std::vector < TH1F > Hee_Njets;
  std::vector < TH1F > Hee_Njets_JES_minus;
  std::vector < TH1F > Hee_Njets_JES_plus;
  std::vector < TH1F > Hee_Njets_JER_minus;
  std::vector < TH1F > Hee_Njets_JER_plus;
  std::vector < TH1F > Hee_Njets_EES_minus;
  std::vector < TH1F > Hee_Njets_EES_plus;
  std::vector < TH1F > Hee_Njets_MES_minus;
  std::vector < TH1F > Hee_Njets_MES_plus;
  std::vector < TH1F > HMet_ee;
  std::vector < TH1F > Hee_NBjets;
  std::vector < TH1F > HCount_ee;
  std::vector < TH1F > HCount_ee_after1btag;
  std::vector < TH1F > Hee_Njets_after1btag;
  //SFb
  std::vector < TH2D > HCount2D_ee_sfb_0b;
  std::vector < TH2D > HCount2D_ee_sfb_1b;
  std::vector < TH2D > HCount2D_ee_sfb_2b;
  std::vector < TH2D > HCount2D_ee_sfb_3b;
  std::vector < TH2D > HWeight2D_ee_sfb_0b;
  std::vector < TH2D > HWeight2D_ee_sfb_1b;
  std::vector < TH2D > HWeight2D_ee_sfb_2b;
  std::vector < TH2D > HWeight2D_ee_sfb_3b;
  //SFl
  std::vector < TH2D > HCount2D_ee_sfl_0b;
  std::vector < TH2D > HCount2D_ee_sfl_1b;
  std::vector < TH2D > HCount2D_ee_sfl_2b;
  std::vector < TH2D > HCount2D_ee_sfl_3b;
  std::vector < TH2D > HWeight2D_ee_sfl_0b;
  std::vector < TH2D > HWeight2D_ee_sfl_1b;
  std::vector < TH2D > HWeight2D_ee_sfl_2b;
  std::vector < TH2D > HWeight2D_ee_sfl_3b;
  //  
  std::vector < TH2D > Hee_NjetsNBjets;
  std::vector < TH2D > Hee_NjetsNBjets_JES_minus;
  std::vector < TH2D > Hee_NjetsNBjets_JES_plus;
  std::vector < TH2D > Hee_NjetsNBjets_JER_minus;
  std::vector < TH2D > Hee_NjetsNBjets_JER_plus;
  std::vector < TH2D > Hee_NjetsNBjets_EES_minus;
  std::vector < TH2D > Hee_NjetsNBjets_EES_plus;
  std::vector < TH2D > Hee_NjetsNBjets_MES_minus;
  std::vector < TH2D > Hee_NjetsNBjets_MES_plus;
  //endmodifdg

  //Background estimation methods
  DYEstimation DYest;
  //the following objects has to be internalized in the class itself
  std::vector < TH1F * >hlistemu;
  std::vector < TH1F * >hlistSR;
  std::vector < TH1F * >hlistCR;

  int nbins = 200;
  float minx = 0.;
  float maxx = 350;

  TH1F *halllistemu = new TH1F ("halllistemu", "halllistemu", nbins, minx, maxx);
  TH1F *hemudata = new TH1F ("hemudata", "hemudata", nbins, minx, maxx);
  TH1F *halllistSR = new TH1F ("halllistSR", "halllistSR", nbins, minx, maxx);
  TH1F *halllistCR = new TH1F ("halllistCR", "halllistCR", nbins, minx, maxx);
  TH1F *hDataSR = new TH1F ("hDataSR", "hDataSR", nbins, minx, maxx);
  TH1F *hDataCR = new TH1F ("hDataCR", "hDataCR", nbins, minx, maxx);

  TH1F *NjetsDataCR = new TH1F ("NjetsDataCR", "NjetsDataCR", 5, -0.5, 4.5);
  TH1F *NjetsDataSR = new TH1F ("NjetsDataSR", "NjetsDataSR", 5, -0.5, 4.5);
  TH1F *NjetsMCCR = new TH1F ("NjetsMCCR", "NjetsMCCR", 5, -0.5, 4.5);
  TH1F *NjetsMCSR = new TH1F ("NjetsMCSR", "NjetsMCSR", 5, -0.5, 4.5);
  //////////////////////

  cout << "The verbosity mode is " << verbosity << endl;
  cout << "The luminosity is equal to " << Luminosity << endl;
  cout << "The DataType is ";
  switch (DataType) {
  case 0:
    cout << "MC" << endl;
    break;
  case 1:
    cout << "Data" << endl;
    break;
  case 2:
    cout << "Data & MC" << endl;
    break;
  default:
    cout << " unknown" << endl;
    break;
  }
  cout << "SF for trigger mu " << SF_trig_mu << endl;
  cout << "SF for trigger emu " << SF_trig_emu << endl;
  cout << "SF for electronID " << SF_e << endl;
  //////////////////////



  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Loop over the datasets  " << endl;
    cout << "#########################" << endl;
  }

// Creation of histogram list
  for (unsigned int d = 0; d < datasets.size (); d++) {
    hlistemu = DYest.CreateDYHisto ("emu", datasets[d].Name (), nbins, minx, maxx);
    hlistSR = DYest.CreateDYHisto ("SR", datasets[d].Name (), nbins, minx, maxx);
    hlistCR = DYest.CreateDYHisto ("CR", datasets[d].Name (), nbins, minx, maxx);
  }
  bool FoundWjets = false;
  for (unsigned int j = 0; j < hlistSR.size (); j++) {
    if (hlistSR[j]->GetName () == string ("WjetsSR"))
      FoundWjets = true;
  }
  if (!FoundWjets) {
    hlistSR = DYest.CreateDYHisto ("SR", "Wjets", nbins, minx, maxx);
    hlistCR = DYest.CreateDYHisto ("CR", "Wjets", nbins, minx, maxx);
    hlistemu = DYest.CreateDYHisto ("emu", "Wjets", nbins, minx, maxx);
  }
  bool FoundVV = false;
  for (unsigned int j = 0; j < hlistSR.size (); j++) {
    if (hlistSR[j]->GetName () == string ("VVSR"))
      FoundVV = true;
  }
  if (!FoundVV) {
    hlistSR = DYest.CreateDYHisto ("SR", "VV", nbins, minx, maxx);
    hlistCR = DYest.CreateDYHisto ("CR", "VV", nbins, minx, maxx);
    hlistemu = DYest.CreateDYHisto ("emu", "VV", nbins, minx, maxx);
  }


  for (unsigned int d = 0; d < datasets.size (); d++) {


    datasets[d].eventTree ()->SetBranchAddress ("NTEvent", &event);

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;


    MyhistoManager.CreateHisto (MyHistos, "InvDilMassNJinf1", datasets[d].Name (), "Mll", "Entries", 50, 0., 350);
    MyhistoManager.CreateHisto (MyHistos1, "InvDilMassNJsup1", datasets[d].Name (), "Mll", "Entries", 50, 0., 350);
    MyhistoManager.CreateHisto (HInvM_ll_pair, "InvDilMassPair", datasets[d].Name (), "Mll", "Entries", 50, 0., 350);

    MyhistoManager.CreateHisto (HInvM_emu_pair, "InvemuMassPair", datasets[d].Name (), "Mll", "Entries", 50, 0., 350);
    MyhistoManager.CreateHisto (Hemu_Njets, "emu_Njets", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_JES_minus, "emu_Njets_JES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_JES_plus, "emu_Njets_JES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_JER_minus, "emu_Njets_JER-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_JER_plus, "emu_Njets_JER-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_EES_minus, "emu_Njets_EES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_EES_plus, "emu_Njets_EES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_MES_minus, "emu_Njets_MES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hemu_Njets_MES_plus, "emu_Njets_MES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (HMet_emu, "Met_emu", datasets[d].Name (), "Met", "Entries", 30, 0, 150);
    MyhistoManager.CreateHisto (Hemu_NBjets, "emu_NBjets", datasets[d].Name (), "NBjets", "Entries", 4, -0.5, 3.5);
    MyhistoManager.CreateHisto (HCount_emu, "Count_emu", datasets[d].Name (), "Njets", "Entries", 1, -0.5, 0.5);
    MyhistoManager.CreateHisto (HCount_emu_after1btag, "Count_emu_after1btag", datasets[d].Name (), "Njets", "Entries", 1, -0.5, 0.5);
    MyhistoManager.CreateHisto (Hemu_Njets_after1btag, "emu_Njets_after1btag", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    //SFb
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfb_0b, "Count2D_emu_sfb_0b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfb_1b, "Count2D_emu_sfb_1b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfb_2b, "Count2D_emu_sfb_2b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfb_3b, "Count2D_emu_sfb_3b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfb_0b, "Weight2D_emu_sfb_0b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfb_1b, "Weight2D_emu_sfb_1b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfb_2b, "Weight2D_emu_sfb_2b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfb_3b, "Weight2D_emu_sfb_3b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    //SFl
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfl_0b, "Count2D_emu_sfl_0b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfl_1b, "Count2D_emu_sfl_1b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfl_2b, "Count2D_emu_sfl_2b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_emu_sfl_3b, "Count2D_emu_sfl_3b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfl_0b, "Weight2D_emu_sfl_0b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfl_1b, "Weight2D_emu_sfl_1b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfl_2b, "Weight2D_emu_sfl_2b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_emu_sfl_3b, "Weight2D_emu_sfl_3b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    //
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets, "emu_NjetsNBjets", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_JES_minus, "emu_NjetsNBjets_JES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_JES_plus, "emu_NjetsNBjets_JES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_JER_minus, "emu_NjetsNBjets_JER-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_JER_plus, "emu_NjetsNBjets_JER-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_EES_minus, "emu_NjetsNBjets_EES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_EES_plus, "emu_NjetsNBjets_EES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_MES_minus, "emu_NjetsNBjets_MES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hemu_NjetsNBjets_MES_plus, "emu_NjetsNBjets_MES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);


    MyhistoManager.CreateHisto (HInvM_mumu_pair, "InvmumuMassPair", datasets[d].Name (), "Mll", "Entries", 50, 0., 350);
    MyhistoManager.CreateHisto (Hmumu_Njets, "mumu_Njets", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_JES_minus, "mumu_Njets_JES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_JES_plus, "mumu_Njets_JES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_JER_minus, "mumu_Njets_JER-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_JER_plus, "mumu_Njets_JER-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_EES_minus, "mumu_Njets_EES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_EES_plus, "mumu_Njets_EES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_MES_minus, "mumu_Njets_MES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_MES_plus, "mumu_Njets_MES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (HMet_mumu, "Met_mumu", datasets[d].Name (), "Met", "Entries", 30, 0, 150);
    MyhistoManager.CreateHisto (Hmumu_NBjets, "mumu_NBjets", datasets[d].Name (), "NBjets", "Entries", 4, -0.5, 3.5);
    MyhistoManager.CreateHisto (HCount_mumu, "Count_mumu", datasets[d].Name (), "Njets", "Entries", 1, -0.5, 0.5);
    MyhistoManager.CreateHisto (HCount_mumu_after1btag, "Count_mumu_after1btag", datasets[d].Name (), "Njets", "Entries", 1, -0.5, 0.5);
    MyhistoManager.CreateHisto (Hmumu_Njets_after1btag, "mumu_Njets_after1btag", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    //SFb
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfb_0b, "Count2D_mumu_sfb_0b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfb_1b, "Count2D_mumu_sfb_1b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfb_2b, "Count2D_mumu_sfb_2b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfb_3b, "Count2D_mumu_sfb_3b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfb_0b, "Weight2D_mumu_sfb_0b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfb_1b, "Weight2D_mumu_sfb_1b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfb_2b, "Weight2D_mumu_sfb_2b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfb_3b, "Weight2D_mumu_sfb_3b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    //SFl
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfl_0b, "Count2D_mumu_sfl_0b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfl_1b, "Count2D_mumu_sfl_1b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfl_2b, "Count2D_mumu_sfl_2b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_mumu_sfl_3b, "Count2D_mumu_sfl_3b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfl_0b, "Weight2D_mumu_sfl_0b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfl_1b, "Weight2D_mumu_sfl_1b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfl_2b, "Weight2D_mumu_sfl_2b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_mumu_sfl_3b, "Weight2D_mumu_sfl_3b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    //
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets, "mumu_NjetsNBjets", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_JES_minus, "mumu_NjetsNBjets_JES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_JES_plus, "mumu_NjetsNBjets_JES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_JER_minus, "mumu_NjetsNBjets_JER-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_JER_plus, "mumu_NjetsNBjets_JER-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_EES_minus, "mumu_NjetsNBjets_EES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_EES_plus, "mumu_NjetsNBjets_EES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_MES_minus, "mumu_NjetsNBjets_MES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hmumu_NjetsNBjets_MES_plus, "mumu_NjetsNBjets_MES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);




    MyhistoManager.CreateHisto (HInvM_ee_pair, "InveeMassPair", datasets[d].Name (), "Mll", "Entries", 50, 0., 350);
    MyhistoManager.CreateHisto (Hee_Njets, "ee_Njets", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_JES_minus, "ee_Njets_JES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_JES_plus, "ee_Njets_JES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_JER_minus, "ee_Njets_JER-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_JER_plus, "ee_Njets_JER-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_EES_minus, "ee_Njets_EES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_EES_plus, "ee_Njets_EES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_MES_minus, "ee_Njets_MES-minus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (Hee_Njets_MES_plus, "ee_Njets_MES-plus", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto (HMet_ee, "Met_ee", datasets[d].Name (), "Met", "Entries", 30, 0, 150);
    MyhistoManager.CreateHisto (Hee_NBjets, "ee_NBjets", datasets[d].Name (), "NBjets", "Entries", 4, -0.5, 3.5);
    MyhistoManager.CreateHisto (HCount_ee, "Count_ee", datasets[d].Name (), "Njets", "Entries", 1, -0.5, 0.5);
    MyhistoManager.CreateHisto (HCount_ee_after1btag, "Count_ee_after1btag", datasets[d].Name (), "Njets", "Entries", 1, -0.5, 0.5);
    MyhistoManager.CreateHisto (Hee_Njets_after1btag, "ee_Njets_after1btag", datasets[d].Name (), "Njets", "Entries", 5, -0.5, 4.5);
    //SFb
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfb_0b, "Count2D_ee_sfb_0b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfb_1b, "Count2D_ee_sfb_1b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfb_2b, "Count2D_ee_sfb_2b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfb_3b, "Count2D_ee_sfb_3b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfb_0b, "Weight2D_ee_sfb_0b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfb_1b, "Weight2D_ee_sfb_1b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfb_2b, "Weight2D_ee_sfb_2b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfb_3b, "Weight2D_ee_sfb_3b", datasets[d].Name (), "SFb", nb_valsfb, valsfb[0]-(valsfb[1]-valsfb[0])/2.,valsfb[nb_valsfb-1]+(valsfb[1]-valsfb[0])/2., "Njets", 5, -0.5, 4.5);
    //SFl
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfl_0b, "Count2D_ee_sfl_0b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfl_1b, "Count2D_ee_sfl_1b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfl_2b, "Count2D_ee_sfl_2b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HCount2D_ee_sfl_3b, "Count2D_ee_sfl_3b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfl_0b, "Weight2D_ee_sfl_0b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfl_1b, "Weight2D_ee_sfl_1b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfl_2b, "Weight2D_ee_sfl_2b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (HWeight2D_ee_sfl_3b, "Weight2D_ee_sfl_3b", datasets[d].Name (), "SFl", nb_valsfl, valsfl[0]-(valsfl[1]-valsfl[0])/2.,valsfl[nb_valsfl-1]+(valsfl[1]-valsfl[0])/2., "Njets", 5, -0.5, 4.5);
    //
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets, "ee_NjetsNBjets", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_JES_minus, "ee_NjetsNBjets_JES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_JES_plus, "ee_NjetsNBjets_JES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_JER_minus, "ee_NjetsNBjets_JER-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_JER_plus, "ee_NjetsNBjets_JER-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_EES_minus, "ee_NjetsNBjets_EES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_EES_plus, "ee_NjetsNBjets_EES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_MES_minus, "ee_NjetsNBjets_MES-minus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);
    MyhistoManager.CreateHisto2D (Hee_NjetsNBjets_MES_plus, "ee_NjetsNBjets_MES-plus", datasets[d].Name (), "Njets", 5, -0.5, 4.5, "NBjets", 5, -0.5, 4.5);



    //////////////////////
    //LOOP OVER THE EVENTS
    //////////////////////
    //for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
    for (unsigned int ievt = 0; ievt < datasets[d].NofEvtsToRunOver(); ievt++) {

      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);

      //      cout << "event number=" << event->eventNb << endl;
      //      cout << "normfactor=" << datasets[d].NormFactor() << endl;
      //      cout << "Xsection=" << datasets[d].Xsection() << endl;
      //      cout << "NofEvtsToRunOver=" << datasets[d].NofEvtsToRunOver() << endl;


      if (ievt % 10000 == 0)
	cout << "number of processed events " << ievt << endl;
      //if(ievt > 50000) break;
      //Load event for the selection
      sel.LoadEvent (event);

      //Collection of selected objects
      vector < NTVertex > selVertices = sel.GetSelectedVertex ();
      vector < NTElectron > selElectrons = sel.GetSelectedElectrons ();
      vector < NTMuon > selMuons = sel.GetSelectedMuons ();
      vector < NTJet > selJets = sel.GetSelectedJets ();
      NTMET met = sel.GetMET ();
      vector < NTElectron > selElectronsEESMinus = sel.GetSelectedElectrons (true, SysEES.m1sigma);
      vector < NTMuon > selMuonsMESMinus = sel.GetSelectedMuons (true, SysMES.m1sigma);
      vector < NTElectron > selElectronsEESPlus = sel.GetSelectedElectrons (true, SysEES.p1sigma);
      vector < NTMuon > selMuonsMESPlus = sel.GetSelectedMuons (true, SysMES.p1sigma);
	

      //Candidate pair of lepton
      string CandType;		// ee - emu - mumum or false
      vector < NTElectron > candElec;
      vector < NTMuon > candMuon;



      //Manage DY samples to avoid overlaps
      double dileptInvMass = 0;
      if ((event->zAndDecays).size () > 0) {
	TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
	dileptInvMass = dilept.M ();
      }

      if (datasets[d].Name () == "Zjets" && dileptInvMass < 50)
	cout << "problem !!!" << endl;

      if (datasets[d].Name () == "Zjets" && dileptInvMass < 50)
	continue;

      if (datasets[d].Name () == "DYToMuMu_M-20" && (dileptInvMass > 50 || dileptInvMass < 20))
	continue;
      if (datasets[d].Name () == "DYToEE_M-20" && (dileptInvMass > 50 || dileptInvMass < 20))
	continue;
      if (datasets[d].Name () == "DYToTauTau_M-20" && (dileptInvMass > 50 || dileptInvMass < 20))
	continue;
      if (datasets[d].Name () == "DYToMuMu_M-10To20" && dileptInvMass > 20)
	continue;
      if (datasets[d].Name () == "DYToEE_M-10To20" && dileptInvMass > 20)
	continue;
      if (datasets[d].Name () == "DYToTauTau_M-10To20" && dileptInvMass > 20)
	continue;



      //Dweight[ITypeMC] = Luminosity*datasets[d].Xsection()/datasets[d].NofEvtsToRunOver();
      //cout << " getNSkimmedEvent " << datasets[d].getNSkimmedEvent() << endl;
      //cout << " NofEvtsToRunOver " << datasets[d].NofEvtsToRunOver() << endl;


      double weightITypeMC_save = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
      double weightITypeMC = 0;


/*
      double SF_e=0.965; //0.965; //1;
      if(ChannelName == "ee" || ChannelName == "emu"){
        if ( datasets[d].Name()=="TTbar" &&  
            (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200  ) )Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
        if ( datasets[d].Name()=="Zjets"              || 
	     datasets[d].Name()=="DYToMuMu_M-20"      || 
	     datasets[d].Name()=="DYToEE_M-20"        || 
	     datasets[d].Name()=="DYToTauTau_M-20"    || 
	     datasets[d].Name()=="DYToMuMu_M-10To20"  || 
	     datasets[d].Name()=="DYToEE_M-10To20"    || 
	     datasets[d].Name()=="DYToTauTau_M-10To20"
	     )Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
      
        if ( datasets[d].Name()=="WZ" || datasets[d].Name()=="WW" || datasets[d].Name()=="ZZ"  )Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
        if ( datasets[d].Name()=="WjetsMu" ||
             datasets[d].Name()=="WjetsE"  ||
             datasets[d].Name()=="WjetsTau" 
         ) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
      
        if ( datasets[d].Name()=="TTbar" &&
            (event->TMEME == 1 || event->TMEME == 10100) )Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
	  
        if ( datasets[d].Name()=="SingleToptW" ) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
      }
*/

//Eric: 09/06
/*
      if (datasets[d].Name () == "DataEG" || datasets[d].Name () == "DataMu" || datasets[d].Name () == "DataMuEG")
	weightITypeMC = 1;
      int LastStep = 0;
      bool IsData = false;
*/

//DYEstimation: pas d'interet ds cette macro, a enlever partout
    /*
      if (sel.passTriggerSelection (&datasets[d], "emu")) {
	sel.GetLeptonPair (candMuon, candElec, CandType);
	if (CandType == "emu" && selVertices.size () > 0) {
	  if (sel.GetSelectedJets (candMuon, candElec).size () >= 2)
	    DYest.FillDYHistos ("emu", hlistemu, datasets[d].Name (), event->TMEME, sel.DiLeptonMass (candMuon, candElec), Dweight[ITypeMC]);
	}
      }
   */
   
/* Eric: 09/06
     for (unsigned int cn = 0; cn < LoopChannelName.size (); cn++) {
      if (datasets[d].Name () == "TTbar") {
	if (LoopChannelName[cn] == "mumu") {
	  if (event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000)
	    IsSignal[(int)cn] = true;
	  if (!(event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000))
	    IsSignal[(int)cn] = false;
	}
	if (LoopChannelName[cn] == "ee") {
	  if (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200)
	    IsSignal[(int)cn] = true;
	  if (!(event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200))
	    IsSignal[(int)cn] = false;
	}
	if (LoopChannelName[cn] == "emu") {
	  if (event->TMEME == 11 || event->TMEME == 21100 || event->TMEME == 11001 || event->TMEME == 10110)
	    IsSignal[(int)cn] = true;
	  if (!(event->TMEME == 11 || event->TMEME == 21100 || event->TMEME == 11001 || event->TMEME == 10110))
	    IsSignal[(int)cn] = false;
	}
	}


      if (datasets[d].Name () == "TTbar") {
	if (IsSignal[(int)cn]) {
	  ITypeMC = 0;
	  Dweight[ITypeMC] = weightITypeMC;
	  TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	  TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
	}
	else {
	  ITypeMC = 1;
	  Dweight[ITypeMC] = weightITypeMC;
	  TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	  TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
	}
      }
      if (datasets[d].Name () == "Zjets" ||
	  datasets[d].Name () == "DYToMuMu_M-20" || datasets[d].Name () == "DYToEE_M-20" || datasets[d].Name () == "DYToTauTau_M-20" || datasets[d].Name () == "DYToMuMu_M-10To20" || datasets[d].Name () == "DYToEE_M-10To20" || datasets[d].Name () == "DYToTauTau_M-10To20") {
	ITypeMC = 2;
	IsSignal[(int)cn] = false;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      if (datasets[d].Name () == "WjetsMu" || datasets[d].Name () == "WjetsE" || datasets[d].Name () == "WjetsTau") {
	ITypeMC = 3;
	IsSignal[(int)cn] = false;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      if (datasets[d].Name () == "SingleToptW") {
	ITypeMC = 4;
	IsSignal[(int)cn] = false;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      if (datasets[d].Name () == "WZ" || datasets[d].Name () == "WW" || datasets[d].Name () == "ZZ") {
	ITypeMC = 5;
	IsSignal[(int)cn] = false;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      if (datasets[d].Name () == "DYee") {
	ITypeMC = 6;
	IsSignal[(int)cn] = false;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      if (datasets[d].Name () == "DYmumu") {
	ITypeMC = 7;
	IsSignal[(int)cn] = false;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      if (datasets[d].Name () == "QCD1" ||
	  datasets[d].Name () == "QCD2" ||
	  datasets[d].Name () == "QCD3" ||
	  datasets[d].Name () == "QCD4" ||
	  datasets[d].Name () == "QCD5" ||
	  datasets[d].Name () == "QCD6" || datasets[d].Name () == "QCD7" || datasets[d].Name () == "QCD8" || datasets[d].Name () == "QCD9" || datasets[d].Name () == "QCD10" || datasets[d].Name () == "QCD11" || datasets[d].Name () == "QCD12" || datasets[d].Name () == "QCD13") {
	ITypeMC = 8;
	IsSignal[(int)cn] = false;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      if (datasets[d].Name () == "DataEG" || datasets[d].Name () == "DataMu") {
	ITypeMC = 100;
	IsData = true;
	Dweight[ITypeMC] = weightITypeMC;
	TabFlow1[cn][ITypeMC][0] += Dweight[ITypeMC];
	TabFlow2[cn][ITypeMC][0] += Dweight[ITypeMC] * Dweight[ITypeMC];
      }
      //}//Loop over channels
*/

      //for (int cn=0; cn<3; cn++) {
      for (unsigned int cn = 0; cn < LoopChannelName.size (); cn++) {
//      for (int cn=1; cn<2; cn++) {
          string ChannelName;
          if (cn==0) ChannelName= "mumu"; 
          else if (cn==1) ChannelName= "ee"; 
          else if (cn==2) ChannelName= "emu"; 


          if (cn==0 && (datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMuEG")) continue;
          if (cn==1 && (datasets[d].Name()=="DataMu" || datasets[d].Name()=="DataMuEG")) continue;
          if (cn==2 && (datasets[d].Name()=="DataMu" || datasets[d].Name()=="DataEG")) continue;

          if ( datasets[d].Name()!="DataEG" && datasets[d].Name()!="DataMu" && 
           datasets[d].Name()!="DataMuEG" && datasets[d].Name()!="DataEGMu") {
	   //*
           if (cn==2) weightITypeMC = weightITypeMC_save*SF_VxP1[selVertices.size()];   
           else if (cn==0) weightITypeMC = weightITypeMC_save*SF_VxP2[selVertices.size()];
           else if (cn==1) weightITypeMC = weightITypeMC_save*SF_VxP3[selVertices.size()];
	   //*/
	    //Now replaced by:
	   //weightITypeMC = weightITypeMC_save*sel.GetPUWeight();
          }
          else { // data   
            weightITypeMC = 1;
          }
      
          //bool IsSignal[cn] = false;
          int LastStep = 0;
          bool IsData = false;
      
          if ( datasets[d].Name()=="TTbar" ) {
            if ( cn==0) { // "mumu" 
	      if ( event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000 )    IsSignal[cn] = true;
	      if ( !(event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000) ) IsSignal[cn] = false;
	    }      
            else if ( cn==1) {  // "ee" 
	      if ( event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200 )     IsSignal[cn] = true;
	      if ( !(event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200) )  IsSignal[cn] = false;
	    }      
            else if ( cn==2) { // "emu" 
	      if ( event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110 )     IsSignal[cn] = true;
	      if ( !(event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110) )  IsSignal[cn] = false;
	    }      
          }
          
          
          if ( datasets[d].Name()=="TTbar" ) { 
            if (IsSignal[cn] ) { ITypeMC = 0;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC]; 
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];} 
	    else     { ITypeMC = 1; Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];}
          }
          else if ( datasets[d].Name()=="Zjets"              || 
	       datasets[d].Name()=="DYToMuMu_M-20"      || 
	       datasets[d].Name()=="DYToEE_M-20"        || 
	       datasets[d].Name()=="DYToTauTau_M-20"    || 
	       datasets[d].Name()=="DYToMuMu_M-10To20"  || 
	       datasets[d].Name()=="DYToEE_M-10To20"    || 
	       datasets[d].Name()=="DYToTauTau_M-10To20"
	       ) { 
            ITypeMC = 2; IsSignal[cn] = false; Dweight[ITypeMC]= weightITypeMC;  
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="WjetsMu" ||
               datasets[d].Name()=="WjetsE"  ||
               datasets[d].Name()=="WjetsTau" ||
	       datasets[d].Name()=="Wjets"
           ) { 
            ITypeMC = 3; IsSignal[cn] = false;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="SingleToptW" ) { 
            ITypeMC = 4; IsSignal[cn] = false;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="WZ" || datasets[d].Name()=="WW" || datasets[d].Name()=="ZZ"  || datasets[d].Name()=="VV") { 
            ITypeMC = 5; IsSignal[cn] = false;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="DYee" ) { 
            ITypeMC = 6; IsSignal[cn] = false;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="DYmumu" ) { 
            ITypeMC = 7; IsSignal[cn] = false;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="QCD1" ||
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
            ITypeMC = 8; IsSignal[cn] = false;  Dweight[ITypeMC]= weightITypeMC;
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }

          if ( datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMu" || 
               datasets[d].Name()=="DataMuEG" || datasets[d].Name()=="DataEGMu") { 
            ITypeMC = 100; IsData = true;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[cn][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[cn][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
	//}
      //}//Loop over channels


//Loop  over the channels

      //for (unsigned int cn = 0; cn < LoopChannelName.size (); cn++) {

// Passage trigger
	// Voir DileptonSelection.cc pour conditions de trigger
	if (sel.passTriggerSelection (&datasets[d], LoopChannelName[cn])) {


	  TabFlow1[cn][ITypeMC][10] += Dweight[ITypeMC];
	  TabFlow2[cn][ITypeMC][10] += Dweight[ITypeMC] * Dweight[ITypeMC];
	  if (LoopChannelName[cn] == "mumu" && !IsData)
	    Dweight[ITypeMC] *= SF_trig_mu;
	  if (LoopChannelName[cn] == "emu" && !IsData)
	    Dweight[ITypeMC] *= SF_trig_emu;

	  //cout << "after trigger " << endl;
	  TabFlow1[cn][ITypeMC][1] += Dweight[ITypeMC];
	  TabFlow2[cn][ITypeMC][1] += Dweight[ITypeMC] * Dweight[ITypeMC];

//      sel.GetLeptonPair(candMuon, candElec, CandType ); 
	  sel.GetLeptonPair (selMuons, selElectrons, candMuon, candElec, CandType);

// Passage dilepton candidat
	  if (((CandType == "mumu" && LoopChannelName[cn] == "mumu") || (CandType == "ee" && LoopChannelName[cn] == "ee") || (CandType == "emu" && LoopChannelName[cn] == "emu")
	      ) && selVertices.size () > 0) {

	    TabFlow1[cn][ITypeMC][11] += Dweight[ITypeMC];
	    TabFlow2[cn][ITypeMC][11] += Dweight[ITypeMC] * Dweight[ITypeMC];


	    if (LoopChannelName[cn] == "ee" && !IsData) {
	      if (datasets[d].Name () == "TTbar") {
		// ee
		if (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200)
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e * SF_e;
		// emu
		if (event->TMEME == 11 || event->TMEME == 21100 || event->TMEME == 11001 || event->TMEME == 10110)
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
		// ejet
		if (event->TMEME == 1 || event->TMEME == 10100)
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
	      }
	      if (datasets[d].Name () == "Zjets" || datasets[d].Name () == "DYToEE_M-20" || datasets[d].Name () == "DYToTauTau_M-20" || datasets[d].Name () == "DYToEE_M-10To20" || datasets[d].Name () == "DYToTauTau_M-10To20")
		Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e * SF_e;
	      if (datasets[d].Name () == "WZ" || datasets[d].Name () == "WW" || datasets[d].Name () == "ZZ")
		Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e * SF_e;
	      if (datasets[d].Name () == "WjetsE" || datasets[d].Name () == "WjetsTau")
		Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
	      if (datasets[d].Name () == "SingleToptW")
		Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e * SF_e;
	    }


	    int nrecoel = selElectrons.size ();
	    if (LoopChannelName[cn] == "emu" && !IsData) {
	      if (datasets[d].Name () == "TTbar") {
		// ee
		if (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200) {
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
		  if (nrecoel > 1)
		    Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
		}
		// emu
		if (event->TMEME == 11 || event->TMEME == 21100 || event->TMEME == 11001 || event->TMEME == 10110)
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
		// ejet
		if (event->TMEME == 1 || event->TMEME == 10100)
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
	      }
	      if (
		   // datasets[d].Name()=="Zjets"              --> hyp: real mu, fake e
		   datasets[d].Name () == "DYToEE_M-20" || datasets[d].Name () == "DYToEE_M-10To20"
		   // DYTauTau -->> hyp : fake e
		) {
		Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
		if (nrecoel > 1)
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
	      }
	      if (datasets[d].Name () == "WZ" || datasets[d].Name () == "WW" || datasets[d].Name () == "ZZ") {
		Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
		if (nrecoel > 1)
		  Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
	      }
//              if ( datasets[d].Name()=="WjetsE"  || datasets[d].Name()=="WjetsTau" ) --> hyp : real mu, fake e
	      if (datasets[d].Name () == "SingleToptW")
		Dweight[ITypeMC] = Dweight[ITypeMC] * SF_e;
	    }



	    TabFlow1[cn][ITypeMC][2] += Dweight[ITypeMC];
	    TabFlow2[cn][ITypeMC][2] += Dweight[ITypeMC] * Dweight[ITypeMC];

	    TLorentzVector DiLepton_mumu;
	    for (unsigned int i = 0; i < candMuon.size (); i++)
	      DiLepton_mumu += candMuon[i].p4;
	    TLorentzVector DiLepton_ee;
	    for (unsigned int i = 0; i < candElec.size (); i++)
	      DiLepton_ee += candElec[i].p4;
	    TLorentzVector DiLepton_emu;
	    for (unsigned int i = 0; i < candMuon.size (); i++)
	      DiLepton_emu += candMuon[i].p4;
	    for (unsigned int i = 0; i < candElec.size (); i++)
	      DiLepton_emu += candElec[i].p4;

	    //Moving Lepton Energy Scale
	    //    EES - minus
	    vector < NTElectron > candElec_EES_Minus;
	    vector < NTMuon > candMuon_EES_Minus;
	    string CandType_EES_Minus;
	    sel.GetLeptonPair (selMuons, selElectronsEESMinus, candMuon_EES_Minus, candElec_EES_Minus, CandType_EES_Minus);
	    //    EES - plus
	    vector < NTElectron > candElec_EES_Plus;
	    vector < NTMuon > candMuon_EES_Plus;
	    string CandType_EES_Plus;
	    sel.GetLeptonPair (selMuons, selElectronsEESPlus, candMuon_EES_Plus, candElec_EES_Plus, CandType_EES_Plus);
	    //    MES - minus
	    vector < NTElectron > candElec_MES_Minus;
	    vector < NTMuon > candMuon_MES_Minus;
	    string CandType_MES_Minus;
	    sel.GetLeptonPair (selMuonsMESMinus, selElectrons, candMuon_MES_Minus, candElec_MES_Minus, CandType_MES_Minus);
	    //    MES - plus
	    vector < NTElectron > candElec_MES_Plus;
	    vector < NTMuon > candMuon_MES_Plus;
	    string CandType_MES_Plus;
	    sel.GetLeptonPair (selMuonsMESPlus, selElectrons, candMuon_MES_Plus, candElec_MES_Plus, CandType_MES_Plus);

	    TLorentzVector DiLepton_mumu_MES_Minus;
	    TLorentzVector DiLepton_mumu_MES_Plus;
	    TLorentzVector DiLepton_mumu_EES_Minus;
	    TLorentzVector DiLepton_mumu_EES_Plus;
	    TLorentzVector DiLepton_emu_MES_Minus;
	    TLorentzVector DiLepton_emu_MES_Plus;
	    TLorentzVector DiLepton_emu_EES_Minus;
	    TLorentzVector DiLepton_emu_EES_Plus;
	    TLorentzVector DiLepton_ee_MES_Minus;
	    TLorentzVector DiLepton_ee_MES_Plus;
	    TLorentzVector DiLepton_ee_EES_Minus;
	    TLorentzVector DiLepton_ee_EES_Plus;
	    //EES - minus
	    for (unsigned int i = 0; i < candMuon_EES_Minus.size (); i++)
	      DiLepton_mumu_EES_Minus += candMuon_EES_Minus[i].p4;
	    for (unsigned int i = 0; i < candElec_EES_Minus.size (); i++)
	      DiLepton_ee_EES_Minus += candElec_EES_Minus[i].p4;
	    for (unsigned int i = 0; i < candMuon_EES_Minus.size (); i++)
	      DiLepton_emu_EES_Minus += candMuon_EES_Minus[i].p4;
	    for (unsigned int i = 0; i < candElec_EES_Minus.size (); i++)
	      DiLepton_emu_EES_Minus += candElec_EES_Minus[i].p4;
	    //EES - plus
	    for (unsigned int i = 0; i < candMuon_EES_Plus.size (); i++)
	      DiLepton_mumu_EES_Plus += candMuon_EES_Plus[i].p4;
	    for (unsigned int i = 0; i < candElec_EES_Plus.size (); i++)
	      DiLepton_ee_EES_Plus += candElec_EES_Plus[i].p4;
	    for (unsigned int i = 0; i < candMuon_EES_Plus.size (); i++)
	      DiLepton_emu_EES_Plus += candMuon_EES_Plus[i].p4;
	    for (unsigned int i = 0; i < candElec_EES_Plus.size (); i++)
	      DiLepton_emu_EES_Plus += candElec_EES_Plus[i].p4;
	    //MES - minus
	    for (unsigned int i = 0; i < candMuon_MES_Minus.size (); i++)
	      DiLepton_mumu_MES_Minus += candMuon_MES_Minus[i].p4;
	    for (unsigned int i = 0; i < candElec_MES_Minus.size (); i++)
	      DiLepton_ee_MES_Minus += candElec_MES_Minus[i].p4;
	    for (unsigned int i = 0; i < candMuon_MES_Minus.size (); i++)
	      DiLepton_emu_MES_Minus += candMuon_MES_Minus[i].p4;
	    for (unsigned int i = 0; i < candElec_MES_Minus.size (); i++)
	      DiLepton_emu_MES_Minus += candElec_MES_Minus[i].p4;
	    //MES - plus
	    for (unsigned int i = 0; i < candMuon_MES_Plus.size (); i++)
	      DiLepton_mumu_MES_Plus += candMuon_MES_Plus[i].p4;
	    for (unsigned int i = 0; i < candElec_MES_Plus.size (); i++)
	      DiLepton_ee_MES_Plus += candElec_MES_Plus[i].p4;
	    for (unsigned int i = 0; i < candMuon_MES_Plus.size (); i++)
	      DiLepton_emu_MES_Plus += candMuon_MES_Plus[i].p4;
	    for (unsigned int i = 0; i < candElec_MES_Plus.size (); i++)
	      DiLepton_emu_MES_Plus += candElec_MES_Plus[i].p4;


	    float InvDilMass = 0;
	    if (LoopChannelName[cn] == "mumu")
	      InvDilMass = DiLepton_mumu.M ();
	    if (LoopChannelName[cn] == "ee")
	      InvDilMass = DiLepton_ee.M ();
	    if (LoopChannelName[cn] == "emu")
	      InvDilMass = DiLepton_emu.M ();

	    vector < NTJet > theselJets = sel.GetSelectedJets (candMuon, candElec);
	    vector < NTJet > theselJetsEESMinus = sel.GetSelectedJets (candMuon_EES_Minus, candElec_EES_Minus);
	    vector < NTJet > theselJetsEESPlus = sel.GetSelectedJets (candMuon_EES_Plus, candElec_EES_Plus);
	    vector < NTJet > theselJetsMESMinus = sel.GetSelectedJets (candMuon_MES_Minus, candElec_MES_Minus);
	    vector < NTJet > theselJetsMESPlus = sel.GetSelectedJets (candMuon_MES_Plus, candElec_MES_Plus);
	    vector < NTJet > theselScaledMinusJets = sel.GetSelectedJets (candMuon, candElec, true, SysJES.m1sigma, false, 0);
	    vector < NTJet > theselScaledPlusJets = sel.GetSelectedJets (candMuon, candElec, true, SysJES.p1sigma, false, 0);
	    vector < NTJet > theselResdMinusJets = sel.GetSelectedJets (candMuon, candElec, false, 0., true, SysJER.m1sigma);
	    vector < NTJet > theselResdPlusJets = sel.GetSelectedJets (candMuon, candElec, false, 0., true, SysJER.p1sigma);

	    if (cn == 0) {	//do it once
	      MyhistoManager.FillHisto (HInvM_ll_pair, "InvDilMassPair", InvDilMass, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);

	      if (theselJets.size () <= 1)
		MyhistoManager.FillHisto (MyHistos, "InvDilMassNJinf1", InvDilMass, datasets[d].Name (), IsSignal[(int)cn], weight);
	      if (theselJets.size () > 1)
		MyhistoManager.FillHisto (MyHistos1, "InvDilMassNJsup1", InvDilMass, datasets[d].Name (), IsSignal[(int)cn], weight);
	    }
//        int iChannel = -1;
//        if (LoopChannelName[cn]=="ee") iChannel = 0;
//        if (LoopChannelName[cn]=="mumu") iChannel = 0;
//        //   std::cout<<"je passe au remplissage"<<std::endl;
//        float wei = -1; 
//        if ( Dweight[ITypeMC]<=0 ) wei = 1.;
//        MyhistoManager.Fill( iChannel, 2, d, 0, InvDilMass, wei);
	    //endmodifdg


	    int JetMul = theselJets.size ();
	    if (JetMul > 3)
	      JetMul = 4;
	    int JetMul_JES_minus = theselScaledMinusJets.size ();
	    if (JetMul_JES_minus > 3)
	      JetMul_JES_minus = 4;
	    int JetMul_JES_plus = theselScaledPlusJets.size ();
	    if (JetMul_JES_plus > 3)
	      JetMul_JES_plus = 4;
	    int JetMul_JER_minus = theselResdMinusJets.size ();
	    if (JetMul_JER_minus > 3)
	      JetMul_JER_minus = 4;
	    int JetMul_JER_plus = theselResdPlusJets.size ();
	    if (JetMul_JER_plus > 3)
	      JetMul_JER_plus = 4;
	    int JetMul_EES_minus = theselJetsEESMinus.size ();
	    if (JetMul_EES_minus > 3)
	      JetMul_EES_minus = 4;
	    int JetMul_EES_plus = theselJetsEESPlus.size ();
	    if (JetMul_EES_plus > 3)
	      JetMul_EES_plus = 4;
	    int JetMul_MES_minus = theselJetsMESMinus.size ();
	    if (JetMul_MES_minus > 3)
	      JetMul_MES_minus = 4;
	    int JetMul_MES_plus = theselJetsMESPlus.size ();
	    if (JetMul_MES_plus > 3)
	      JetMul_MES_plus = 4;

	    float misset = met.p4.Et ();

	    /////////////////////////////////////
	    //Count the number of b-jets 
	    /////////////////////////////////////
	    int NBtaggedJets = 0;
	    int NBtaggedJets_JES_Minus = 0;
	    int NBtaggedJets_JES_Plus = 0;
	    int NBtaggedJets_JER_Minus = 0;
	    int NBtaggedJets_JER_Plus = 0;
	    int NBtaggedJets_EES_Minus = 0;
	    int NBtaggedJets_EES_Plus = 0;
	    int NBtaggedJets_MES_Minus = 0;
	    int NBtaggedJets_MES_Plus = 0;
	    int AlgoBtag = sel.GetbtagAlgo ();
	    for (unsigned int j = 0; j < theselJets.size (); j++) {
	      if (AlgoBtag == 0 && theselJets[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets++;
	      if (AlgoBtag == 1 && theselJets[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets++;
	      if (AlgoBtag == 2 && theselJets[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets++;
	    }
	    for (unsigned int j = 0; j < theselScaledMinusJets.size (); j++) {
	      if (AlgoBtag == 0 && theselScaledMinusJets[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JES_Minus++;
	      if (AlgoBtag == 1 && theselScaledMinusJets[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JES_Minus++;
	      if (AlgoBtag == 2 && theselScaledMinusJets[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JES_Minus++;
	    }
	    for (unsigned int j = 0; j < theselScaledPlusJets.size (); j++) {
	      if (AlgoBtag == 0 && theselScaledPlusJets[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JES_Plus++;
	      if (AlgoBtag == 1 && theselScaledPlusJets[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JES_Plus++;
	      if (AlgoBtag == 2 && theselScaledPlusJets[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JES_Plus++;
	    }
	    for (unsigned int j = 0; j < theselResdMinusJets.size (); j++) {
	      if (AlgoBtag == 0 && theselResdMinusJets[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JER_Minus++;
	      if (AlgoBtag == 1 && theselResdMinusJets[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JER_Minus++;
	      if (AlgoBtag == 2 && theselResdMinusJets[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JER_Minus++;
	    }
	    for (unsigned int j = 0; j < theselResdPlusJets.size (); j++) {
	      if (AlgoBtag == 0 && theselResdPlusJets[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JER_Plus++;
	      if (AlgoBtag == 1 && theselResdPlusJets[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JER_Plus++;
	      if (AlgoBtag == 2 && theselResdPlusJets[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_JER_Plus++;
	    }
	    for (unsigned int j = 0; j < theselJetsEESMinus.size (); j++) {
	      if (AlgoBtag == 0 && theselJetsEESMinus[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_EES_Minus++;
	      if (AlgoBtag == 1 && theselJetsEESMinus[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_EES_Minus++;
	      if (AlgoBtag == 2 && theselJetsEESMinus[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_EES_Minus++;
	    }
	    for (unsigned int j = 0; j < theselJetsEESPlus.size (); j++) {
	      if (AlgoBtag == 0 && theselJetsEESPlus[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_EES_Plus++;
	      if (AlgoBtag == 1 && theselJetsEESPlus[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_EES_Plus++;
	      if (AlgoBtag == 2 && theselJetsEESPlus[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_EES_Plus++;
	    }
	    for (unsigned int j = 0; j < theselJetsMESMinus.size (); j++) {
	      if (AlgoBtag == 0 && theselJetsMESMinus[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_MES_Minus++;
	      if (AlgoBtag == 1 && theselJetsMESMinus[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_MES_Minus++;
	      if (AlgoBtag == 2 && theselJetsMESMinus[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_MES_Minus++;
	    }
	    for (unsigned int j = 0; j < theselJetsMESPlus.size (); j++) {
	      if (AlgoBtag == 0 && theselJetsMESPlus[j].TCDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_MES_Plus++;
	      if (AlgoBtag == 1 && theselJetsMESPlus[j].SVDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_MES_Plus++;
	      if (AlgoBtag == 2 && theselJetsMESPlus[j].SMDiscri >= sel.GetbtagDiscriCut ())
		NBtaggedJets_MES_Plus++;
	    }
// weightb
	  
	    vector < float >weightb;
	    vector < float >weightb_JES_Minus;
	    vector < float >weightb_JES_Plus;
	    vector < float >weightb_JER_Minus;
	    vector < float >weightb_JER_Plus;
	    vector < float >weightb_EES_Minus;
	    vector < float >weightb_EES_Plus;
	    vector < float >weightb_MES_Minus;
	    vector < float >weightb_MES_Plus;
	    InitVectorOfWeight(weightb);	
	    InitVectorOfWeight(weightb_JES_Minus);	
	    InitVectorOfWeight(weightb_JES_Plus);	
	    InitVectorOfWeight(weightb_JER_Minus);	
	    InitVectorOfWeight(weightb_JER_Plus);	
	    InitVectorOfWeight(weightb_EES_Minus);	
	    InitVectorOfWeight(weightb_EES_Plus);	
	    InitVectorOfWeight(weightb_MES_Minus);	
	    InitVectorOfWeight(weightb_MES_Plus);	
	    if (sel.GetFlagb () == 1) {
	      if (!IsData) {	//MC 
		LoadBWeight(sel, weightb, theselJets);
		LoadBWeight(sel, weightb_JES_Minus, theselScaledMinusJets);
		LoadBWeight(sel, weightb_JES_Plus, theselScaledPlusJets);
		LoadBWeight(sel, weightb_JER_Minus, theselResdMinusJets);
		LoadBWeight(sel, weightb_JER_Plus, theselResdPlusJets);
		LoadBWeight(sel, weightb_EES_Minus, theselJetsEESMinus);
		LoadBWeight(sel, weightb_EES_Plus, theselJetsEESPlus);
		LoadBWeight(sel, weightb_MES_Minus, theselJetsMESMinus);
		LoadBWeight(sel, weightb_MES_Plus, theselJetsMESPlus);
	      }
/*
             else {  //data
               if (NBtaggedJets >= sel.GetNofBtagJetsCut()) {
                  weightb[0] = 1;
               }
               else  {
                  weightb[0]=0;
               }
              for (int ibj=0; ibj<4; ibj++) {
                 if ( NBtaggedJets== ibj) weightb[ibj+1] = 1;
                 else weightb[ibj] = 0;
              }
              if ( NBtaggedJets>=3) weightb[4] = 1;
             }
*/

	    }

	    int NBtaggedJets3 = NBtaggedJets;
	    if (NBtaggedJets3 > 3)
	      NBtaggedJets3 = 3;
	    int NBtaggedJets3_JES_Minus = NBtaggedJets_JES_Minus;
	    if (NBtaggedJets3_JES_Minus > 3)
	      NBtaggedJets3_JES_Minus = 3;
	    int NBtaggedJets3_JES_Plus = NBtaggedJets_JES_Plus;
	    if (NBtaggedJets3_JES_Plus > 3)
	      NBtaggedJets3_JES_Plus = 3;
	    int NBtaggedJets3_JER_Minus = NBtaggedJets_JER_Minus;
	    if (NBtaggedJets3_JER_Minus > 3)
	      NBtaggedJets3_JER_Minus = 3;
	    int NBtaggedJets3_JER_Plus = NBtaggedJets_JER_Minus;
	    if (NBtaggedJets3_JER_Plus > 3)
	      NBtaggedJets3_JER_Plus = 3;
	    int NBtaggedJets3_EES_Minus = NBtaggedJets_EES_Minus;
	    if (NBtaggedJets3_EES_Minus > 3)
	      NBtaggedJets3_EES_Minus = 3;
	    int NBtaggedJets3_EES_Plus = NBtaggedJets_EES_Plus;
	    if (NBtaggedJets3_EES_Plus > 3)
	      NBtaggedJets3_EES_Plus = 3;
	    int NBtaggedJets3_MES_Minus = NBtaggedJets_MES_Minus;
	    if (NBtaggedJets3_MES_Minus > 3)
	      NBtaggedJets3_MES_Minus = 3;
	    int NBtaggedJets3_MES_Plus = NBtaggedJets_MES_Plus;
	    if (NBtaggedJets3_MES_Plus > 3)
	      NBtaggedJets3_MES_Plus = 3;
	    /* 
	       if(CandType=="mumu" && LoopChannelName[cn]=="mumu"){
	       MyhistoManager.FillHisto(HInvM_mumu_pair, "InvmumuMassPair" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hmumu_Njets,     "mumu_Njets"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(HMet_mumu,       "Met_mumu"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if (sel.GetFlagb() == 0 || IsData) {
	       MyhistoManager.FillHisto(Hmumu_NBjets,    "mumu_NBjets"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       }
	       else if (sel.GetFlagb() == 1) {
	       MyhistoManager.FillHisto(Hmumu_NBjets,    "mumu_NBjets"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hmumu_NBjets,    "mumu_NBjets"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hmumu_NBjets,    "mumu_NBjets"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hmumu_NBjets,    "mumu_NBjets"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
	       }
	       }
	       if(CandType=="ee" && LoopChannelName[cn]=="ee"){
	       MyhistoManager.FillHisto(HInvM_ee_pair, "InveeMassPair" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hee_Njets,     "ee_Njets"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(HMet_ee,       "Met_ee"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if (sel.GetFlagb() == 0 || IsData) {
	       MyhistoManager.FillHisto(Hee_NBjets,    "ee_NBjets"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       }
	       else if (sel.GetFlagb() == 1) {
	       MyhistoManager.FillHisto(Hee_NBjets,    "ee_NBjets"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hee_NBjets,    "ee_NBjets"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hee_NBjets,    "ee_NBjets"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hee_NBjets,    "ee_NBjets"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
	       }
	       }
	       if(CandType=="emu" && LoopChannelName[cn]=="emu"){
	       MyhistoManager.FillHisto(HInvM_emu_pair, "InvemuMassPair" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hemu_Njets,     "emu_Njets"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(HMet_emu,       "Met_emu"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if (sel.GetFlagb() == 0 || IsData) {
	       MyhistoManager.FillHisto(Hemu_NBjets,    "emu_NBjets"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       }
	       else if (sel.GetFlagb() == 1) {
	       MyhistoManager.FillHisto(Hemu_NBjets,    "emu_NBjets"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hemu_NBjets,    "emu_NBjets"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hemu_NBjets,    "emu_NBjets"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	       MyhistoManager.FillHisto(Hemu_NBjets,    "emu_NBjets"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
	       }
	       }
	     */

	    if (cn == 0) {	//do it once
//DYEstimation
	      if (InvDilMass > 20 && (LoopChannelName[cn] == "ee" || LoopChannelName[cn] == "mumu") && CandType == LoopChannelName[cn]) {
		bool isinSR = false;
		bool isinCR = false;

// cut au niveau dilepton pair ou apres la coupure en met
//              if ( theselJets.size()>1 ) {
		if (theselJets.size () > 1 && met.p4.Et () > sel.GetMETCut ().second) {

		  DYest.FillDYHistos ("SR", hlistSR, datasets[d].Name (), event->TMEME, sel.DiLeptonMass (candMuon, candElec), Dweight[ITypeMC]);
		  isinSR = true;
		}

// cut au niveau dilepton pair ou apres la coupure en met
//              if ( theselJets.size()<=1 ) { 
		if (theselJets.size () <= 1 && met.p4.Et () > sel.GetMETCut ().second) {

		  DYest.FillDYHistos ("CR", hlistCR, datasets[d].Name (), event->TMEME, sel.DiLeptonMass (candMuon, candElec), Dweight[ITypeMC]);
		  isinCR = true;
		}

// cut au niveau dilepton pair ou apres la coupure en met
		if (met.p4.Et () > sel.GetMETCut ().second) {
		  DYest.FillNbofEventsInSRandCR (isinCR, isinSR, sel.DiLeptonMass (candMuon, candElec), datasets[d].Name (), Dweight[ITypeMC]);
		  if (isinCR && IsData)
		    NjetsDataCR->Fill (theselJets.size (), Dweight[ITypeMC]);
		  if (isinSR && IsData)
		    NjetsDataSR->Fill (theselJets.size (), Dweight[ITypeMC]);
		  if (isinCR && !IsData)
		    NjetsMCCR->Fill (theselJets.size (), Dweight[ITypeMC]);
		  if (isinSR && !IsData)
		    NjetsMCSR->Fill (theselJets.size (), Dweight[ITypeMC]);
		}
	      }
	    }

// Passage invariante mass dilepton
//        if (( InvDilMass<sel.GetZmassWindowCut().first || InvDilMass>sel.GetZmassWindowCut().second ) && (InvDilMass> sel.GetMinValueMassCut() ))  {
	    if (((LoopChannelName[cn] == "mumu" || LoopChannelName[cn] == "ee") && sel.DiLeptonMassCut (sel.GetMinValueMassCut (), sel.GetZmassWindowCut (), candMuon, candElec, LoopChannelName[cn])) || (LoopChannelName[cn] == "emu" && InvDilMass > sel.GetMinValueMassCut ())
	      ) {

	      TabFlow1[cn][ITypeMC][3] += Dweight[ITypeMC];
	      TabFlow2[cn][ITypeMC][3] += Dweight[ITypeMC] * Dweight[ITypeMC];

// Passage multiplicite de jets
	      if (theselJets.size () >= 2) {
		TabFlow1[cn][ITypeMC][4] += Dweight[ITypeMC];
		TabFlow2[cn][ITypeMC][4] += Dweight[ITypeMC] * Dweight[ITypeMC];

// Passage met
		if (((LoopChannelName[cn] == "mumu" || LoopChannelName[cn] == "ee") && met.p4.Et () > sel.GetMETCut ().second) || (LoopChannelName[cn] == "emu" && met.p4.Et () > sel.GetMETCut ().first)) {
		  TabFlow1[cn][ITypeMC][5] += Dweight[ITypeMC];
		  TabFlow2[cn][ITypeMC][5] += Dweight[ITypeMC] * Dweight[ITypeMC];

		   //if(IsSignal[(int)cn]) cout<<"*Here: "<<IsSignal[(int)cn]<<" "<<Dweight[ITypeMC]<<endl;
		  /////////////////////////////////////////////
		  //........... CHANNEL MUMU ................//
		  /////////////////////////////////////////////
		  //EES-MES
		  if (CandType_EES_Minus == "mumu" && LoopChannelName[cn] == "mumu") {
		    if (JetMul_EES_minus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_EES_minus, "mumu_Njets_EES-minus", JetMul_EES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_EES_minus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_EES_minus, "mumu_NjetsNBjets_EES-minus", JetMul_EES_minus, NBtaggedJets3_EES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_EES_Plus == "mumu" && LoopChannelName[cn] == "mumu") {
		    if (JetMul_EES_plus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_EES_plus, "mumu_Njets_EES-plus", JetMul_EES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_EES_plus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_EES_plus, "mumu_NjetsNBjets_EES-plus", JetMul_EES_plus, NBtaggedJets3_EES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_MES_Minus == "mumu" && LoopChannelName[cn] == "mumu") {
		    if (JetMul_MES_minus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_MES_minus, "mumu_Njets_MES-minus", JetMul_MES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_MES_minus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_MES_minus, "mumu_NjetsNBjets_MES-minus", JetMul_MES_minus, NBtaggedJets3_MES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_MES_Plus == "mumu" && LoopChannelName[cn] == "mumu") {
		    if (JetMul_MES_plus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_MES_plus, "mumu_Njets_MES-plus", JetMul_MES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_MES_plus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_MES_plus, "mumu_NjetsNBjets_MES-plus", JetMul_MES_plus, NBtaggedJets3_MES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  //////////////////////
		  if (CandType == "mumu" && LoopChannelName[cn] == "mumu") {
		    MyhistoManager.FillHisto (HInvM_mumu_pair, "InvmumuMassPair", InvDilMass, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (Hmumu_Njets, "mumu_Njets", JetMul, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JES_minus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_JES_minus, "mumu_Njets_JES-minus", JetMul_JES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JES_plus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_JES_plus, "mumu_Njets_JES-plus", JetMul_JES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JER_minus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_JER_minus, "mumu_Njets_JER-minus", JetMul_JER_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JER_plus >= 2)
		      MyhistoManager.FillHisto (Hmumu_Njets_JER_plus, "mumu_Njets_JER-plus", JetMul_JER_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (HMet_mumu, "Met_mumu", misset, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (HCount_mumu, "Count_mumu", 0, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      MyhistoManager.FillHisto (Hmumu_NBjets, "mumu_NBjets", NBtaggedJets3, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      float NjetsNBjets = JetMul + 5 * NBtaggedJets3;	// 5 = nombre de bins de Hmumu_Njets
		      float NjetsNBjets_JES_minus = JetMul_JES_minus + 5 * NBtaggedJets3_JES_Minus;
		      float NjetsNBjets_JES_plus = JetMul_JES_plus + 5 * NBtaggedJets3_JES_Plus;
		      float NjetsNBjets_JER_minus = JetMul_JER_minus + 5 * NBtaggedJets3_JER_Minus;
		      float NjetsNBjets_JER_plus = JetMul_JER_plus + 5 * NBtaggedJets3_JER_Minus;
		      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets, "mumu_NjetsNBjets", JetMul, NBtaggedJets3, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JES_minus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JES_minus, "mumu_NjetsNBjets_JES-minus", JetMul_JES_minus, NBtaggedJets3_JES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JES_plus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JES_plus, "mumu_NjetsNBjets_JES-plus", JetMul_JES_plus, NBtaggedJets3_JES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JER_minus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JER_minus, "mumu_NjetsNBjets_JER-minus", JetMul_JER_minus, NBtaggedJets3_JER_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JER_plus >= 2)
			MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JER_plus, "mumu_NjetsNBjets_JER-plus", JetMul_JER_plus, NBtaggedJets3_JER_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		    else if (sel.GetFlagb () == 1) {
		      MyhistoManager.FillHisto (Hmumu_NBjets, "mumu_NBjets", 0, datasets[d].Name (), IsSignal[(int)cn], weightb[1] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hmumu_NBjets, "mumu_NBjets", 1, datasets[d].Name (), IsSignal[(int)cn], weightb[2] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hmumu_NBjets, "mumu_NBjets", 2, datasets[d].Name (), IsSignal[(int)cn], weightb[3] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hmumu_NBjets, "mumu_NBjets", 3, datasets[d].Name (), IsSignal[(int)cn], weightb[4] * Dweight[ITypeMC]);
		    }
		  }

		  /////////////////////////////////////////////
		  //........... CHANNEL EE  .................//
		  /////////////////////////////////////////////
		  //EES-MES
		  if (CandType_EES_Minus == "ee" && LoopChannelName[cn] == "ee") {
		    if (JetMul_EES_minus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_EES_minus, "ee_Njets_EES-minus", JetMul_EES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_EES_minus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_EES_minus, "ee_NjetsNBjets_EES-minus", JetMul_EES_minus, NBtaggedJets3_EES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_EES_Plus == "ee" && LoopChannelName[cn] == "ee") {
		    if (JetMul_EES_plus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_EES_plus, "ee_Njets_EES-plus", JetMul_EES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_EES_plus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_EES_plus, "ee_NjetsNBjets_EES-plus", JetMul_EES_plus, NBtaggedJets3_EES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_MES_Minus == "ee" && LoopChannelName[cn] == "ee") {
		    if (JetMul_MES_minus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_MES_minus, "ee_Njets_MES-minus", JetMul_MES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_MES_minus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_MES_minus, "ee_NjetsNBjets_MES-minus", JetMul_MES_minus, NBtaggedJets3_MES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_MES_Plus == "ee" && LoopChannelName[cn] == "ee") {
		    if (JetMul_MES_plus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_MES_plus, "ee_Njets_MES-plus", JetMul_MES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_MES_plus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_MES_plus, "ee_NjetsNBjets_MES-plus", JetMul_MES_plus, NBtaggedJets3_MES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  //////////////////////
		  if (CandType == "ee" && LoopChannelName[cn] == "ee") {
		    cout<<"THERE "<<IsSignal[(int)cn]<<" "<<Dweight[ITypeMC]<<endl;
		    if(IsSignal[(int)cn]) cout<<"There: "<<IsSignal[(int)cn]<<" "<<Dweight[ITypeMC]<<endl;
		    MyhistoManager.FillHisto (HInvM_ee_pair, "InveeMassPair", InvDilMass, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (Hee_Njets, "ee_Njets", JetMul, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JES_minus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_JES_minus, "ee_Njets_JES-minus", JetMul_JES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JES_plus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_JES_plus, "ee_Njets_JES-plus", JetMul_JES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JER_minus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_JER_minus, "ee_Njets_JER-minus", JetMul_JER_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JER_plus >= 2)
		      MyhistoManager.FillHisto (Hee_Njets_JER_plus, "ee_Njets_JER-plus", JetMul_JER_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (HMet_ee, "Met_ee", misset, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (HCount_ee, "Count_ee", 0, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      MyhistoManager.FillHisto (Hee_NBjets, "ee_NBjets", NBtaggedJets3, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      float NjetsNBjets = JetMul + 5 * NBtaggedJets3;	// 5 = nombre de bins de Hee_Njets
		      float NjetsNBjets_JES_minus = JetMul_JES_minus + 5 * NBtaggedJets3;
		      float NjetsNBjets_JES_plus = JetMul_JES_plus + 5 * NBtaggedJets3;
		      float NjetsNBjets_JER_minus = JetMul_JER_minus + 5 * NBtaggedJets3;
		      float NjetsNBjets_JER_plus = JetMul_JER_plus + 5 * NBtaggedJets3;
		      MyhistoManager.FillHisto2D (Hee_NjetsNBjets, "ee_NjetsNBjets", JetMul, NBtaggedJets3, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JES_minus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JES_minus, "ee_NjetsNBjets_JES-minus", JetMul_JES_minus, NBtaggedJets3_JES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JES_plus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JES_plus, "ee_NjetsNBjets_JES-plus", JetMul_JES_plus, NBtaggedJets3_JES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JER_minus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JER_minus, "ee_NjetsNBjets_JER-minus", JetMul_JER_minus, NBtaggedJets3_JER_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JER_plus >= 2)
			MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JER_plus, "ee_NjetsNBjets_JER-plus", JetMul_JER_plus, NBtaggedJets3_JER_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		    else if (sel.GetFlagb () == 1) {
		      MyhistoManager.FillHisto (Hee_NBjets, "ee_NBjets", 0, datasets[d].Name (), IsSignal[(int)cn], weightb[1] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hee_NBjets, "ee_NBjets", 1, datasets[d].Name (), IsSignal[(int)cn], weightb[2] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hee_NBjets, "ee_NBjets", 2, datasets[d].Name (), IsSignal[(int)cn], weightb[3] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hee_NBjets, "ee_NBjets", 3, datasets[d].Name (), IsSignal[(int)cn], weightb[4] * Dweight[ITypeMC]);
		    }
		  }

		  /////////////////////////////////////////////
		  //........... CHANNEL EMU .................//
		  /////////////////////////////////////////////
		  //EES-MES
		  if (CandType_EES_Minus == "emu" && LoopChannelName[cn] == "emu") {
		    if (JetMul_EES_minus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_EES_minus, "emu_Njets_EES-minus", JetMul_EES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_EES_minus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_EES_minus, "emu_NjetsNBjets_EES-minus", JetMul_EES_minus, NBtaggedJets3_EES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_EES_Plus == "emu" && LoopChannelName[cn] == "emu") {
		    if (JetMul_EES_plus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_EES_plus, "emu_Njets_EES-plus", JetMul_EES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_EES_plus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_EES_plus, "emu_NjetsNBjets_EES-plus", JetMul_EES_plus, NBtaggedJets3_EES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_MES_Minus == "emu" && LoopChannelName[cn] == "emu") {
		    if (JetMul_MES_minus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_MES_minus, "emu_Njets_MES-minus", JetMul_MES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_MES_minus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_MES_minus, "emu_NjetsNBjets_MES-minus", JetMul_MES_minus, NBtaggedJets3_MES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  if (CandType_MES_Plus == "emu" && LoopChannelName[cn] == "emu") {
		    if (JetMul_MES_plus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_MES_plus, "emu_Njets_MES-plus", JetMul_MES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      if (JetMul_MES_plus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_MES_plus, "emu_NjetsNBjets_MES-plus", JetMul_MES_plus, NBtaggedJets3_MES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		  }
		  //////////////////////
		  if (CandType == "emu" && LoopChannelName[cn] == "emu") {
		    MyhistoManager.FillHisto (HInvM_emu_pair, "InvemuMassPair", InvDilMass, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (Hemu_Njets, "emu_Njets", JetMul, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JES_minus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_JES_minus, "emu_Njets_JES-minus", JetMul_JES_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JES_plus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_JES_plus, "emu_Njets_JES-plus", JetMul_JES_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JER_minus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_JER_minus, "emu_Njets_JER-minus", JetMul_JER_minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (JetMul_JER_plus >= 2)
		      MyhistoManager.FillHisto (Hemu_Njets_JER_plus, "emu_Njets_JER-plus", JetMul_JER_plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (HMet_emu, "Met_emu", misset, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    MyhistoManager.FillHisto (HCount_emu, "Count_emu", 0, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    if (sel.GetFlagb () == 0 || IsData) {
		      MyhistoManager.FillHisto (Hemu_NBjets, "emu_NBjets", NBtaggedJets3, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      /*
		         float NjetsNBjets =  JetMul + 5*NBtaggedJets3 ;  // 5 = nombre de bins de Hemu_Njets
		         float NjetsNBjets_JES_minus =  JetMul_JES_minus + 5*NBtaggedJets3 ;  
		         float NjetsNBjets_JES_plus =  JetMul_JES_plus + 5*NBtaggedJets3 ;  
		         float NjetsNBjets_JER_minus =  JetMul_JER_minus + 5*NBtaggedJets3 ;  
		         float NjetsNBjets_JER_plus =  JetMul_JER_plus + 5*NBtaggedJets3 ;  
		       */
		      cout<<"IsData emu - "<<JetMul<<" "<<NBtaggedJets3<<endl;
		      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets, "emu_NjetsNBjets", JetMul, NBtaggedJets3, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JES_minus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JES_minus, "emu_NjetsNBjets_JES-minus", JetMul_JES_minus, NBtaggedJets3_JES_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JES_plus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JES_plus, "emu_NjetsNBjets_JES-plus", JetMul_JES_plus, NBtaggedJets3_JES_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JER_minus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JER_minus, "emu_NjetsNBjets_JER-minus", JetMul_JER_minus, NBtaggedJets3_JER_Minus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      if (JetMul_JER_plus >= 2)
			MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JER_plus, "emu_NjetsNBjets_JER-plus", JetMul_JER_plus, NBtaggedJets3_JER_Plus, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		    }
		    else if (sel.GetFlagb () == 1) {
		      MyhistoManager.FillHisto (Hemu_NBjets, "emu_NBjets", 0, datasets[d].Name (), IsSignal[(int)cn], weightb[1] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hemu_NBjets, "emu_NBjets", 1, datasets[d].Name (), IsSignal[(int)cn], weightb[2] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hemu_NBjets, "emu_NBjets", 2, datasets[d].Name (), IsSignal[(int)cn], weightb[3] * Dweight[ITypeMC]);
		      MyhistoManager.FillHisto (Hemu_NBjets, "emu_NBjets", 3, datasets[d].Name (), IsSignal[(int)cn], weightb[4] * Dweight[ITypeMC]);
		    }
		  }


// Passage Btagging


/////////////////////////
// For Eff_b vs Sf_b
/////////////////////////

// Mettre le flagb a 1 dans le fichier de config!!

		  if (sel.GetFlagb () == 1 && !IsData) {
		    //Scan of SF_b 
		    for (int sfb_index = 0; sfb_index < nb_valsfb; sfb_index++) {
		      vector < float >weight_temp = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJets, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_JES_minus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselScaledMinusJets, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_JES_plus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselScaledPlusJets, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_JER_minus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselResdMinusJets, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_JER_plus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselResdPlusJets, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_EES_minus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJetsEESMinus, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_EES_plus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJetsEESPlus, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_MES_minus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJetsMESMinus, valsfb[sfb_index], 1.);
		      vector < float >weight_temp_MES_plus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJetsMESPlus, valsfb[sfb_index], 1.);
		      // do not change weightb[0] ; //weight of the event
		      // weight_temp[1];  //  proba 0 jet
		      // weight_temp[2];  //  proba 1 jet
		      // weight_temp[3];  //  proba 2 jets
		      // weight_temp[4];  //  proba at least 3 jets
//                     cout << " sf1 " << valsfb[sfb_index] << " w " << weight_temp[1]<< " "<< weight_temp[2]<<" "<< weight_temp[3]<<std::endl;
                      //EES-MES
		      if (CandType_EES_Minus == "mumu" && LoopChannelName[cn] == "mumu" && valsfb[sfb_index] == 1 && JetMul_EES_minus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_EES_minus, "mumu_NjetsNBjets_EES-minus", JetMul_EES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_EES_minus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_EES_Plus == "mumu" && LoopChannelName[cn] == "mumu" && valsfb[sfb_index] == 1 && JetMul_EES_plus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_EES_plus, "mumu_NjetsNBjets_EES-plus", JetMul_EES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_EES_plus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_MES_Minus == "mumu" && LoopChannelName[cn] == "mumu" && valsfb[sfb_index] == 1 && JetMul_MES_minus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_MES_minus, "mumu_NjetsNBjets_MES-minus", JetMul_MES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_MES_minus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_MES_Plus == "mumu" && LoopChannelName[cn] == "mumu" && valsfb[sfb_index] == 1 && JetMul_MES_plus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_MES_plus, "mumu_NjetsNBjets_MES-plus", JetMul_MES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_MES_plus[k + 1] * Dweight[ITypeMC]);
		      }
		      //
		      cout<<"Info: "<<CandType<<" "<<LoopChannelName[cn]<<endl;
		      if (CandType == "mumu" && LoopChannelName[cn] == "mumu") {
 			//cout<<"passe ici"<<endl;
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfb_0b, "Weight2D_mumu_sfb_0b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[1]);
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfb_1b, "Weight2D_mumu_sfb_1b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[2]);
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfb_2b, "Weight2D_mumu_sfb_2b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[3]);
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfb_3b, "Weight2D_mumu_sfb_3b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[4]);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfb_0b, "Count2D_mumu_sfb_0b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfb_1b, "Count2D_mumu_sfb_1b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfb_2b, "Count2D_mumu_sfb_2b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfb_3b, "Count2D_mumu_sfb_3b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			if (valsfb[sfb_index] == 1) {	// reference value
 			//cout<<"passe la !"<<endl;
			  for (int k = 0; k < 4; k++) {
			    float NjetsNBjets = JetMul + 5 * k;	// 5 = nombre de bins de Hmumu_Njets
			    float NjetsNBjets_JES_minus = JetMul_JES_minus + 5 * k;	// 5 = nombre de bins de Hmumu_Njets
			    float NjetsNBjets_JES_plus = JetMul_JES_plus + 5 * k;	// 5 = nombre de bins de Hmumu_Njets
			    float NjetsNBjets_JER_minus = JetMul_JER_minus + 5 * k;	// 5 = nombre de bins de Hmumu_Njets
			    float NjetsNBjets_JER_plus = JetMul_JER_plus + 5 * k;	// 5 = nombre de bins de Hmumu_Njets
 			    cout<<"pres a remplir mumu !"<<IsSignal[(int)cn]<<" "<<ITypeMC<<" "<<Dweight[ITypeMC]<<" "<<weight_temp[k + 1] * Dweight[ITypeMC]<<endl;
			    MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets, "mumu_NjetsNBjets", JetMul, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JES_minus >= 2)
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JES_minus, "mumu_NjetsNBjets_JES-minus", JetMul_JES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JES_minus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JES_plus >= 2)
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JES_plus, "mumu_NjetsNBjets_JES-plus", JetMul_JES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JES_plus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JER_minus >= 2)
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JER_minus, "mumu_NjetsNBjets_JER-minus", JetMul_JER_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JER_minus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JER_plus >= 2)
			      MyhistoManager.FillHisto2D (Hmumu_NjetsNBjets_JER_plus, "mumu_NjetsNBjets_JER-plus", JetMul_JER_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JER_plus[k + 1] * Dweight[ITypeMC]);
			  }
			}	//valsfb[sfb_index]==1
		      }		//CandType=="mumu" && LoopChannelName[cn]=="mumu"
                      //EES-MES
		      if (CandType_EES_Minus == "ee" && LoopChannelName[cn] == "ee" && valsfb[sfb_index] == 1 && JetMul_EES_minus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_EES_minus, "ee_NjetsNBjets_EES-minus", JetMul_EES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_EES_minus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_EES_Plus == "ee" && LoopChannelName[cn] == "ee" && valsfb[sfb_index] == 1 && JetMul_EES_plus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_EES_plus, "ee_NjetsNBjets_EES-plus", JetMul_EES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_EES_plus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_MES_Minus == "ee" && LoopChannelName[cn] == "ee" && valsfb[sfb_index] == 1 && JetMul_MES_minus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_MES_minus, "ee_NjetsNBjets_MES-minus", JetMul_MES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_MES_minus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_MES_Plus == "ee" && LoopChannelName[cn] == "ee" && valsfb[sfb_index] == 1 && JetMul_MES_plus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_MES_plus, "ee_NjetsNBjets_MES-plus", JetMul_MES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_MES_plus[k + 1] * Dweight[ITypeMC]);
		      }
		      //
		      if (CandType == "ee" && LoopChannelName[cn] == "ee") {
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfb_0b, "Weight2D_ee_sfb_0b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[1]);
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfb_1b, "Weight2D_ee_sfb_1b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[2]);
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfb_2b, "Weight2D_ee_sfb_2b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[3]);
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfb_3b, "Weight2D_ee_sfb_3b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[4]);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfb_0b, "Count2D_ee_sfb_0b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfb_1b, "Count2D_ee_sfb_1b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfb_2b, "Count2D_ee_sfb_2b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfb_3b, "Count2D_ee_sfb_3b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			if (valsfb[sfb_index] == 1) {	// reference value
			  for (int k = 0; k < 4; k++) {
			    float NjetsNBjets = JetMul + 5 * k;	// 5 = nombre de bins de Hee_Njets
			    float NjetsNBjets_JES_minus = JetMul_JES_minus + 5 * k;	// 5 = nombre de bins de Hee_Njets
			    float NjetsNBjets_JES_plus = JetMul_JES_plus + 5 * k;	// 5 = nombre de bins de Hee_Njets
			    float NjetsNBjets_JER_minus = JetMul_JER_minus + 5 * k;	// 5 = nombre de bins de Hee_Njets
			    float NjetsNBjets_JER_plus = JetMul_JER_plus + 5 * k;	// 5 = nombre de bins de Hee_Njets
 			    cout<<"pres a remplir ee !"<<IsSignal[(int)cn]<<" "<<ITypeMC<<" "<<Dweight[ITypeMC]<<" "<<weight_temp[k + 1] * Dweight[ITypeMC]<<endl;
			    MyhistoManager.FillHisto2D (Hee_NjetsNBjets, "ee_NjetsNBjets", JetMul, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JES_minus >= 2)
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JES_minus, "ee_NjetsNBjets_JES-minus", JetMul_JES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JES_minus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JES_plus >= 2)
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JES_plus, "ee_NjetsNBjets_JES-plus", JetMul_JES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JES_plus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JER_minus >= 2)
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JER_minus, "ee_NjetsNBjets_JER-minus", JetMul_JER_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JER_minus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JER_plus >= 2)
			      MyhistoManager.FillHisto2D (Hee_NjetsNBjets_JER_plus, "ee_NjetsNBjets_JER-plus", JetMul_JER_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JER_plus[k + 1] * Dweight[ITypeMC]);
			  }
			}	//valsfb[sfb_index]==1
		      }		//CandType=="ee" && LoopChannelName[cn]=="ee"
                      //EES-MES
		      if (CandType_EES_Minus == "emu" && LoopChannelName[cn] == "emu" && valsfb[sfb_index] == 1 && JetMul_EES_minus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_EES_minus, "emu_NjetsNBjets_EES-minus", JetMul_EES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_EES_minus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_EES_Plus == "emu" && LoopChannelName[cn] == "emu" && valsfb[sfb_index] == 1 && JetMul_EES_plus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_EES_plus, "emu_NjetsNBjets_EES-plus", JetMul_EES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_EES_plus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_MES_Minus == "emu" && LoopChannelName[cn] == "emu" && valsfb[sfb_index] == 1 && JetMul_MES_minus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_MES_minus, "emu_NjetsNBjets_MES-minus", JetMul_MES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_MES_minus[k + 1] * Dweight[ITypeMC]);
		      }
		      if (CandType_MES_Plus == "emu" && LoopChannelName[cn] == "emu" && valsfb[sfb_index] == 1 && JetMul_MES_plus >= 2) {	// reference value
			  for (int k = 0; k < 4; k++) 
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_MES_plus, "emu_NjetsNBjets_MES-plus", JetMul_MES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_MES_plus[k + 1] * Dweight[ITypeMC]);
		      }
		      //
		      if (CandType == "emu" && LoopChannelName[cn] == "emu") {
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfb_0b, "Weight2D_emu_sfb_0b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[1]);
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfb_1b, "Weight2D_emu_sfb_1b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[2]);
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfb_2b, "Weight2D_emu_sfb_2b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[3]);
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfb_3b, "Weight2D_emu_sfb_3b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[4]);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfb_0b, "Count2D_emu_sfb_0b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfb_1b, "Count2D_emu_sfb_1b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfb_2b, "Count2D_emu_sfb_2b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfb_3b, "Count2D_emu_sfb_3b", valsfb[sfb_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			if (valsfb[sfb_index] == 1) {	// reference value
			  for (int k = 0; k < 4; k++) {
			    float NjetsNBjets = JetMul + 5 * k;	// 5 = nombre de bins de Hemu_Njets
			    float NjetsNBjets_JES_minus = JetMul_JES_minus + 5 * k;	// 5 = nombre de bins de Hemu_Njets
			    float NjetsNBjets_JES_plus = JetMul_JES_plus + 5 * k;	// 5 = nombre de bins de Hemu_Njets
			    float NjetsNBjets_JER_minus = JetMul_JER_minus + 5 * k;	// 5 = nombre de bins de Hemu_Njets
			    float NjetsNBjets_JER_plus = JetMul_JER_plus + 5 * k;	// 5 = nombre de bins de Hemu_Njets
 			    cout<<"pres a remplir emu !"<<IsSignal[(int)cn]<<" "<<ITypeMC<<" "<<Dweight[ITypeMC]<<" "<<weight_temp[k + 1] * Dweight[ITypeMC]<<endl;
			    MyhistoManager.FillHisto2D (Hemu_NjetsNBjets, "emu_NjetsNBjets", JetMul, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JES_minus >= 2)
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JES_minus, "emu_NjetsNBjets_JES-minus", JetMul_JES_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JES_minus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JES_plus >= 2)
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JES_plus, "emu_NjetsNBjets_JES-plus", JetMul_JES_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JES_plus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JER_minus >= 2)
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JER_minus, "emu_NjetsNBjets_JER-minus", JetMul_JER_minus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JER_minus[k + 1] * Dweight[ITypeMC]);
			    if (JetMul_JER_plus >= 2)
			      MyhistoManager.FillHisto2D (Hemu_NjetsNBjets_JER_plus, "emu_NjetsNBjets_JER-plus", JetMul_JER_plus, k, datasets[d].Name (), IsSignal[(int)cn], weight_temp_JER_plus[k + 1] * Dweight[ITypeMC]);
			  }
			}	//valsfb[sfb_index]==1
		      }		//CandType=="emu" && LoopChannelName[cn]=="emu"
		    }		// sfb - end loop
		  }		// flag cut
// End For Eff_b vs Sf_b

/////////////////////////
// For Eff_b vs Sf_l {light quarks}
/////////////////////////
		  if (sel.GetFlagb () == 1 && !IsData) {
		    //Scan of SF_b 
		    for (int sfl_index = 0; sfl_index < nb_valsfl; sfl_index++) {
		      vector < float >weight_temp = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJets, 1., valsfl[sfl_index]);
		      vector < float >weight_temp_JES_minus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselScaledMinusJets, 1., valsfl[sfl_index]);
		      vector < float >weight_temp_JES_plus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselScaledPlusJets, 1., valsfl[sfl_index]);
		      vector < float >weight_temp_JER_minus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselResdMinusJets, 1., valsfl[sfl_index]);
		      vector < float >weight_temp_JER_plus = sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselResdPlusJets, 1., valsfl[sfl_index]);
		      // do not change weightb[0] ; //weight of the event
		      // weight_temp[1];  //  proba 0 jet
		      // weight_temp[2];  //  proba 1 jet
		      // weight_temp[3];  //  proba 2 jets
		      // weight_temp[4];  //  proba at least 3 jets
//                     cout << " sf1 " << valsfl[sfl_index] << " w " << weight_temp[1]<< " "<< weight_temp[2]<<" "<< weight_temp[3]<<std::endl;
		      if (CandType == "mumu" && LoopChannelName[cn] == "mumu") {
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfl_0b, "Weight2D_mumu_sfl_0b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[1]);
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfl_1b, "Weight2D_mumu_sfl_1b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[2]);
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfl_2b, "Weight2D_mumu_sfl_2b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[3]);
			MyhistoManager.FillHisto2D (HWeight2D_mumu_sfl_3b, "Weight2D_mumu_sfl_3b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[4]);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfl_0b, "Count2D_mumu_sfl_0b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfl_1b, "Count2D_mumu_sfl_1b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfl_2b, "Count2D_mumu_sfl_2b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_mumu_sfl_3b, "Count2D_mumu_sfl_3b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
		      }		//CandType=="mumu" && LoopChannelName[cn]=="mumu"
		      if (CandType == "ee" && LoopChannelName[cn] == "ee") {
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfl_0b, "Weight2D_ee_sfl_0b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[1]);
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfl_1b, "Weight2D_ee_sfl_1b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[2]);
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfl_2b, "Weight2D_ee_sfl_2b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[3]);
			MyhistoManager.FillHisto2D (HWeight2D_ee_sfl_3b, "Weight2D_ee_sfl_3b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[4]);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfl_0b, "Count2D_ee_sfl_0b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfl_1b, "Count2D_ee_sfl_1b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfl_2b, "Count2D_ee_sfl_2b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_ee_sfl_3b, "Count2D_ee_sfl_3b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
		      }		//CandType=="ee" && LoopChannelName[cn]=="ee"
		      if (CandType == "emu" && LoopChannelName[cn] == "emu") {
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfl_0b, "Weight2D_emu_sfl_0b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[1]);
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfl_1b, "Weight2D_emu_sfl_1b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[2]);
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfl_2b, "Weight2D_emu_sfl_2b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[3]);
			MyhistoManager.FillHisto2D (HWeight2D_emu_sfl_3b, "Weight2D_emu_sfl_3b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], weight_temp[4]);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfl_0b, "Count2D_emu_sfl_0b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfl_1b, "Count2D_emu_sfl_1b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfl_2b, "Count2D_emu_sfl_2b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
			MyhistoManager.FillHisto2D (HCount2D_emu_sfl_3b, "Count2D_emu_sfl_3b", valsfl[sfl_index], JetMul, datasets[d].Name (), IsSignal[(int)cn], 1.);
		      }		//CandType=="emu" && LoopChannelName[cn]=="emu"
		    }		// sfl - end loop
		  }		// flag cut
// End For Eff_b vs Sf_l

		  if (sel.GetFlagb () == 0 || IsData) {
		    if (NBtaggedJets >= 1) {
		      TabFlow1[cn][ITypeMC][6] += Dweight[ITypeMC];
		      TabFlow2[cn][ITypeMC][6] += Dweight[ITypeMC] * Dweight[ITypeMC];
		      if (CandType == "mumu" && LoopChannelName[cn] == "mumu") {
			MyhistoManager.FillHisto (HCount_mumu_after1btag, "Count_mumu_after1btag", 0, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
			MyhistoManager.FillHisto (Hmumu_Njets_after1btag, "mumu_Njets_after1btag", JetMul, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      }
		      if (CandType == "ee" && LoopChannelName[cn] == "ee") {
			MyhistoManager.FillHisto (HCount_ee_after1btag, "Count_ee_after1btag", 0, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
			MyhistoManager.FillHisto (Hee_Njets_after1btag, "ee_Njets_after1btag", JetMul, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      }
		      if (CandType == "emu" && LoopChannelName[cn] == "emu") {
			MyhistoManager.FillHisto (HCount_emu_after1btag, "Count_emu_after1btag", 0, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
			MyhistoManager.FillHisto (Hemu_Njets_after1btag, "emu_Njets_after1btag", JetMul, datasets[d].Name (), IsSignal[(int)cn], Dweight[ITypeMC]);
		      }
		    }
		    if (NBtaggedJets >= 2) {
		      TabFlow1[cn][ITypeMC][7] += Dweight[ITypeMC];
		      TabFlow2[cn][ITypeMC][7] += Dweight[ITypeMC] * Dweight[ITypeMC];
		    }
		  }
		  else if (sel.GetFlagb () == 1) {
		    //n btagged jet==1
		    TabFlow1[cn][ITypeMC][6] += (1 - weightb[1]) * Dweight[ITypeMC];
		    TabFlow2[cn][ITypeMC][6] += (1 - weightb[1]) * Dweight[ITypeMC] * (1 - weightb[1]) * Dweight[ITypeMC];
		    //n btagged jet==2
		    TabFlow1[cn][ITypeMC][7] += weightb[0] * Dweight[ITypeMC];
		    TabFlow2[cn][ITypeMC][7] += weightb[0] * Dweight[ITypeMC] * weightb[0] * Dweight[ITypeMC];
		  }

		}		//met.p4.Et()>X (X==30 for ee/mumu; X==20/0 for emu)

	      }			//theselJets.size()>=2

	    }			// (DiLepton.M()<76 ||  DiLepton.M() >106) && DiLepton.M()>12

	  }			//CandType (Dilepton pair+VxP)

	}			//event->TriggerPassed()  
      }				//loop over channel name 

    }				// end of loop over evts


  }				// end of loop over the datasets 
  cout << "#########################" << endl;
  cout << " Loop over the datasets  " << endl;
  cout << "#########################" << endl;


//DYEstimation 
  DYest.MakeDYHistos ("emu", hlistemu, halllistemu);
  DYest.MakeDYHistos ("SR", hlistSR, halllistSR);
  DYest.MakeDYHistos ("CR", hlistCR, halllistCR);
  DYest.MakeDataHistos ("SR", hlistSR, hDataSR);
  DYest.MakeDataHistos ("CR", hlistCR, hDataCR);


  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

//DYEstimation 
// Fit the emu channel to parametrize the shape of the non-DY events
  vector < double >parafit;
  parafit = DYest.MakeDYFits_emu (halllistemu);
// Fit the mumu or ee channel with the BW shape + parametrized shape of the non-DY events in the Signal Region for MC
// Take into account a scaled fraction of non-DY events for systematics
  std::cout << "Appel a MakeDYFits_SR" << std::endl;
  DYest.MakeDYFits_SR (halllistSR, nbins, minx, maxx, parafit, 0, 0, "MC", 1.);

// Fit the mumu or ee channel with the BW shape in the Control Region for MC 
  std::cout << "Appel a MakeDYFits_CR" << std::endl;
  DYest.MakeDYFits_CR (halllistCR, nbins, minx, maxx, 0, 0, "MC");

  std::cout << "appel a ComputeDYEstimatedEvents_MC" << std::endl;
  DYest.ComputeDYEstimatedEvents_MC ();
  std::cout << "Number of MC DY events in Z peak (for CR) = " << DYest.GetNDYMCinMz_CR () << std::endl;
  std::cout << "Number of MC DY events in Z peak (for SR) = " << DYest.GetNDYMCinMz_SR () << std::endl;
  std::cout << "Number of MC DY events outside the Z peak (for CR) = " << DYest.GetNDYMC_CR () << std::endl;

// MC comparison between real and expected yields
  std::cout << "Number of MC DY events outside the Z peak (for SR) = " << DYest.GetNDYMC_SR () << std::endl;
  std::cout << "Number of ESTIMATED MC DY events outside the Z peak (for SR) = " << DYest.GetNDYEstimatedMC () << std::endl;

// Computation for Data
// Fit the emu channel to parametrize the shape of the non-DY events
  //parafit.clear();
  //parafit = DYest.MakeDYFits_emu(hemudata);
// Fit the mumu or ee channel with the BW shape + parametrized MC shape of the non-DY events in the Signal Region 
// Take into account a scaled fraction of non-DY events for systematics
  std::cout << "Appel a MakeDYFits_SR pour Data" << std::endl;
  DYest.MakeDYFits_SR (hDataSR, nbins, minx, maxx, parafit, 0, 0, "Data", 1.);

// Fit the mumu or ee channel with the BW shape in the Control Region    
  std::cout << "Appel a MakeDYFits_CR pour Data" << std::endl;
  DYest.MakeDYFits_CR (hDataCR, nbins, minx, maxx, 0, 0, "Data");

// Compute the number of expected bckg according to 
// "Counting" method (simply count the number of events in each zones) or from a "Fit" method of the inv.mass distribution (effect of others process bckg for DY can be taken into account)
// "MC" or "DATA" origin for the scale factor (NofEventsintheMZpieak(inSR)/NofEventsintheMZpieak(inCR))
// modifdg
  DYest.ComputeDYEstimatedEvents_DATA ("Counting", "Data");
  std::cout << "Counting Number of Data events outside the Z peak (for SR)                          = " << DYest.GetNDATA_SR () << std::endl;
  std::cout << "Counting Number of ESTIMATED DY events outside the Z peak (for SR)                  =  " << DYest.GetNDYEstimatedDATACounting () << std::endl;
  std::cout << "statistical error =  " << DYest.GetNDYEstimatedStatErrorDATACounting () << std::endl;
// endmodifdg
  DYest.ComputeDYEstimatedEvents_DATA ("Fit", "Data");
  std::cout << "Fitting Number of Data events outside the Z peak (for SR)                          = " << DYest.GetNDATA_SR () << std::endl;
//  std::cout<<"Fitting Number of Data events outside the Z peak (for CR)                          = "<<DYest.GetNFittedDATA_CR() <<std::endl;
// better to count in the non Mz mass range for CR:
  std::cout << "Fitting Number of Data events outside the Z peak (for CR)                          = " << DYest.GetNDATA_CR () << std::endl;
  std::cout << "Fitting Number of ESTIMATED DY events outside the Z peak (for SR)                  = " << DYest.GetNDYEstimatedDATA () << std::endl;
  std::cout << "statistical error =  " << DYest.GetNDYEstimatedStatErrorDATA () << std::endl;
  std::cout << "to be compared with the number of MC DY events outside the Z peak (for SR) = " << DYest.GetNDYMC_SR () << std::endl;

  //modifdg
  std::vector < string > ordre;	// display = from bottom to top
  ordre.push_back ("TTbarBkg");
  ordre.push_back ("WjetsMu");
  ordre.push_back ("WjetsE");
  ordre.push_back ("WjetsTau");
  ordre.push_back ("Zjets");
  ordre.push_back ("DYToMuMu_M-20");
  ordre.push_back ("DYToEE_M-20");
  ordre.push_back ("DYToTauTau_M-20");
  ordre.push_back ("DYToMuMu_M-10To20");
  ordre.push_back ("DYToEE_M-10To20");
  ordre.push_back ("DYToTauTau_M-10To20");
  ordre.push_back ("SingleToptW");
  ordre.push_back ("WW");
  ordre.push_back ("WZ");
  ordre.push_back ("ZZ");
  ordre.push_back ("TTbarSig");

  THStack *hS = new THStack ();
  THStack *hS_HInvM_ll_pair = new THStack ();

  THStack *hS_HInvM_emu_pair = new THStack ();
  THStack *hS_Hemu_Njets = new THStack ();
  THStack *hS_HMet_emu = new THStack ();
  THStack *hS_Hemu_NBjets = new THStack ();

  THStack *hS_HInvM_mumu_pair = new THStack ();
  THStack *hS_Hmumu_Njets = new THStack ();
  THStack *hS_Hmumu_Njets_JES_minus = new THStack ();
  THStack *hS_Hmumu_Njets_JES_plus = new THStack ();
  THStack *hS_HMet_mumu = new THStack ();
  THStack *hS_Hmumu_NBjets = new THStack ();
  THStack *hS_HCount_mumu = new THStack ();
  THStack *hS_HCount_mumu_after1btag = new THStack ();
  THStack *hS_Hmumu_Njets_after1btag = new THStack ();


  THStack *hS_HInvM_ee_pair = new THStack ();
  THStack *hS_Hee_Njets = new THStack ();
  THStack *hS_HMet_ee = new THStack ();
  THStack *hS_Hee_NBjets = new THStack ();

  MyhistoManager.StackHisto (hS, MyHistos, "InvDilMassNJinf1", ordre);


  string dytype = "";
  if (ChannelName == "ee")
    dytype = "DYee";
  if (ChannelName == "mumu")
    dytype = "DYmumu";

  //  MyhistoManager.MergeHistos(MyHistos,"Zjets",dytype);
  //  MyhistoManager.MergeHistos(HInvM_ll_pair,"Zjets",dytype);
  MyhistoManager.StackHisto (hS, MyHistos, "InvDilMassNJinf1", ordre);
  MyhistoManager.StackHisto (hS_HInvM_ll_pair, HInvM_ll_pair, "InvDilMassPair", ordre);


  MyhistoManager.StackHisto (hS_HInvM_emu_pair, HInvM_emu_pair, "InvemuMassPair", ordre);
  MyhistoManager.StackHisto (hS_Hemu_Njets, Hemu_Njets, "emu_Njets", ordre);
  MyhistoManager.StackHisto (hS_HMet_emu, HMet_emu, "Met_emu", ordre);
  MyhistoManager.StackHisto (hS_Hemu_NBjets, Hemu_NBjets, "emu_NBjets", ordre);



  MyhistoManager.StackHisto (hS_HInvM_mumu_pair, HInvM_mumu_pair, "InvmumuMassPair", ordre);
  MyhistoManager.StackHisto (hS_Hmumu_Njets, Hmumu_Njets, "mumu_Njets", ordre);
  MyhistoManager.StackHisto (hS_Hmumu_Njets_JES_minus, Hmumu_Njets_JES_minus, "mumu_Njets_JES-minus", ordre);
  MyhistoManager.StackHisto (hS_Hmumu_Njets_JES_plus, Hmumu_Njets_JES_plus, "mumu_Njets_JES-plus", ordre);
  MyhistoManager.StackHisto (hS_HMet_mumu, HMet_mumu, "Met_mumu", ordre);
  MyhistoManager.StackHisto (hS_Hmumu_NBjets, Hmumu_NBjets, "mumu_NBjets", ordre);
  MyhistoManager.StackHisto (hS_HCount_mumu, HCount_mumu, "Count_mumu", ordre);
  MyhistoManager.StackHisto (hS_HCount_mumu_after1btag, HCount_mumu_after1btag, "Count_mumu_after1btag", ordre);
  MyhistoManager.StackHisto (hS_Hmumu_Njets_after1btag, Hmumu_Njets_after1btag, "mumu_Njets_after1btag", ordre);


  MyhistoManager.StackHisto (hS_HInvM_ee_pair, HInvM_ee_pair, "InveeMassPair", ordre);
  MyhistoManager.StackHisto (hS_Hee_Njets, Hee_Njets, "ee_Njets", ordre);
  MyhistoManager.StackHisto (hS_HMet_ee, HMet_ee, "Met_ee", ordre);
  MyhistoManager.StackHisto (hS_Hee_NBjets, Hee_NBjets, "ee_NBjets", ordre);

  //endmodifdg

  for (int k0 = 0; k0 < 5; ++k0) {
    for (int k1 = 0; k1 < 101; ++k1) {
      for (int k2 = 0; k2 < 101; ++k2) {
	TabFlow2[k0][k1][k2] = sqrt (TabFlow2[k0][k1][k2]);
      }
    }
  }

  //Compute total bckgd
  for (int k0 = 0; k0 < 5; ++k0) {
    for (int k1 = 1; k1 < 10; ++k1) {
      for (int k2 = 0; k2 < 50; ++k2) {
	TabFlow1[k0][50][k2] += TabFlow1[k0][k1][k2];
      }
    }
  }

  for (int k0 = 0; k0 < 5; ++k0) {
    for (int k2 = 1; k2 < 51; ++k2) {
      for (int k1 = 1; k1 < 10; ++k1) {
	TabFlow2[k0][50][k2] += TabFlow2[k0][k1][k2] * TabFlow2[k0][k1][k2];
      }
      TabFlow2[k0][50][k2] = sqrt (TabFlow2[k0][50][k2]);
    }
  }


// compute SF effect CARO
  for (int k0 = 0; k0 < 3; ++k0) {
    for (int k2 = 0; k2 < 12; ++k2) {
      int ityp = k2;
      if (k2 == 10)
	ityp = 50;
      if (k2 == 11)
	ityp = 100;
      if (TabFlow1[k0][ityp][10] > 0)
	TabFlow1[k0][ityp][10] = TabFlow1[k0][ityp][1] / TabFlow1[k0][ityp][10];
      if (TabFlow1[k0][ityp][11] > 0)
	TabFlow1[k0][ityp][11] = TabFlow1[k0][ityp][2] / TabFlow1[k0][ityp][11];
    }
  }

//


  for (int k0 = 0; k0 < 1; ++k0) {
    for (int k1 = 0; k1 < 6; ++k1) {
      for (int k2 = 0; k2 < 6; ++k2) {
	std::cout << "channel,type,cut " << k0 << " " << k1 << " " << k2 << " " << TabFlow1[k0][k1][k2] << " +- " << TabFlow2[k0][k1][k2] << std::endl;
      }
    }
  }

  for (int k0 = 0; k0 < 1; ++k0) {
    for (int k2 = 0; k2 < 6; ++k2) {
      std::cout << "total bkgd " << " " << TabFlow1[k0][50][k2] << " +- " << sqrt (TabFlow2[k0][50][k2]) << std::endl;
    }
  }



  cout << "#########################" << endl;
  cout << " Fill the latex tables   " << endl;
  cout << "#########################" << endl;

  string ofiletexname;
  if(argc>2) ofiletexname = string(argv[2])+".tex";
  else ofiletexname = string("BtaggingFiles.root.tex"); 
  ofstream ofile (ofiletexname.c_str());
  ofile << "\\documentclass[amsmath,amssymb]{revtex4}" << endl;
  ofile << "\\begin{document}" << endl;

  ofile.setf (ios::fixed);
  ofile.precision (3);
/*
  ofile << "\\clearpage" << endl << endl << endl;
  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{center}" << endl;
  
  // Deatailed table
  
  
  // Zjets, Wjets 
  ofile << "\\begin{tabular}{|l|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & TopBackg & Zjets & Wjets   \\\\" << endl;
  ofile << "\\hline" << endl;
  
  for(int ic=0; ic<CutName.size(); ++ic) {
    ofile <<CutName[ic]<<" & "<<  TabFlow1[IChannel][1][ic] << " $\\pm$ "<< TabFlow2[IChannel][1][ic] << " & " << 
      TabFlow1[IChannel][2][ic] << " $\\pm$ "<< TabFlow2[IChannel][2][ic] << " & " << 
      TabFlow1[IChannel][3][ic] << " $\\pm$ "<< TabFlow2[IChannel][3][ic] << 
      " \\\\" << endl;
  }
  ofile << "\\hline " << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  
  
  
  // QCD, singletoptW, Dibosons 
  ofile << "\\begin{tabular}{|l|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut &  QCD &   SingleTop-tW  & Dibosons\\\\" << endl;
  ofile << "\\hline" << endl;
  
  for(int ic=0; ic<CutName.size(); ++ic) {
    ofile <<CutName[ic]<<" & "<<  TabFlow1[IChannel][8][ic]  << " $\\pm$ "<<  TabFlow2[IChannel][8][ic]  << " & " << 
      TabFlow1[IChannel][4][ic] << " $\\pm$ "<< TabFlow2[IChannel][4][ic] << " & " << 
      TabFlow1[IChannel][5][ic] << " $\\pm$ "<< TabFlow2[IChannel][5][ic] << 
      " \\\\" << endl;
  }
  
  ofile << "\\hline " << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  
  
  ofile << "\\end{center}" << endl;
  ofile << "\\end{table}" << endl;
*/

//Loop over channels
  for (unsigned int cn = 0; cn < LoopChannelName.size (); cn++) {

  vector < string > CutName;
  CutName.push_back ("Total");
  CutName.push_back ("Trigger");
  CutName.push_back ("Dilepton pair and VxP");
  if (LoopChannelName[cn] == "ee" || LoopChannelName[cn] == "mumu") {
    CutName.push_back ("M($ll$) $\\notin$ [76,106] and $>$12 GeV$/c^2$");
  }
  if (LoopChannelName[cn] == "emu") {
    CutName.push_back ("M($ll$) $>$ 12 GeV$/c^2$");
  }
  CutName.push_back ("$E_T(jet1,2)$ $>$ 30 GeV");
  if (LoopChannelName[cn] != "emu") {
    CutName.push_back ("$\\not\\!\\!E_T$ $>$ 30 GeV");
  }
  if (LoopChannelName[cn] == "emu") {
    if (sel.GetMETCut ().first > 0)
      CutName.push_back ("$\\not\\!\\!E_T$ $>$ 20 GeV");
    if (sel.GetMETCut ().first <= 0)
      CutName.push_back ("$\\not\\!\\!E_T$ $>$ 0 GeV");
  }
  CutName.push_back ("btag ($\\ge$1)");
  CutName.push_back ("btag ($\\ge$2)");


  // Summary tables
  ofile << "\\clearpage" << endl;
  ofile << "\\begin{table}[p]" << endl;

  ofile << "\\begin{tabular}{|l|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & DATA & TT signal  & Total Background & S/B \\\\" << endl;
  ofile << "\\hline" << endl;

  for (int ic = 0; ic < CutName.size (); ++ic) {
    ofile << CutName[ic] << " & " << TabFlow1[cn][100][ic] << " $\\pm$ " << TabFlow2[cn][100][ic] << " & " <<
      TabFlow1[cn][0][ic] << " $\\pm$ " << TabFlow2[cn][0][ic] << " & " << TabFlow1[cn][50][ic] << " $\\pm$ " << TabFlow2[cn][50][ic] << " & " << TabFlow1[cn][0][ic] / TabFlow1[cn][50][ic] << " \\\\" << endl;
  }

  // summary table
  ofile << "\\hline" << endl;
// ajout Caro
  ofile << "$SF_{trig}$  & " << TabFlow1[cn][100][10] << " & " << TabFlow1[cn][0][10] << " & " << TabFlow1[cn][50][10] << " &  \\\\" << endl;
  ofile << "$SF_{eID}$  & " << TabFlow1[cn][100][11] << " & " << TabFlow1[cn][0][11] << " & " << TabFlow1[cn][50][11] << " &  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
// end ajout Caro
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & TopBackg & SingleTop & DY & Wjets  & Diboson & QCD  \\\\" << endl;
  ofile << "\\hline" << endl;

  for (int ic = 0; ic < CutName.size (); ++ic) {
    double TopBkgd = TabFlow1[cn][1][ic];
    double ErrTopBkgd = TabFlow2[cn][1][ic];
    // to be changed
    //    double DY = DY + mll<50;
    double DY = TabFlow1[cn][2][ic];
    double errDY = (TabFlow2[cn][2][ic] * TabFlow2[cn][2][ic]);
    errDY = sqrt (errDY);
    double Wj = TabFlow1[cn][3][ic];
    double errWjets = TabFlow2[cn][3][ic];
    double Qcd = TabFlow1[cn][8][ic];
    double errQcd = TabFlow2[cn][8][ic];
    double SinglTop = TabFlow1[cn][4][ic];
    double errSinglTop = sqrt (TabFlow2[cn][4][ic] * TabFlow2[cn][4][ic]);
    double Dibos = TabFlow1[cn][5][ic];
    double errDibos = TabFlow2[cn][5][ic];

    ofile << CutName[ic] << " & " <<
      TopBkgd << " $\\pm$ " << ErrTopBkgd << " & " << SinglTop << " $\\pm$ " << errSinglTop << " & " << DY << " $\\pm$ " << errDY << " & " << Wj << " $\\pm$ " << errWjets << " & " << Dibos << " $\\pm$ " << errDibos << " & " << Qcd << " $\\pm$ " << errQcd << " \\\\" << endl;
  }

  ofile << "\\hline " << endl;
// ajout Caro
  ofile << "$SF_{trig}$  & " << TabFlow1[cn][1][10] << " & " << TabFlow1[cn][4][10] << " & " << TabFlow1[cn][2][10] << " & " << TabFlow1[cn][3][10] << " & " << TabFlow1[cn][5][10] << " & " << TabFlow1[cn][8][10] << " \\\\" << endl;
  ofile << "$SF_{eID}$  & " << TabFlow1[cn][1][11] << " & " << TabFlow1[cn][4][11] << " & " << TabFlow1[cn][2][11] << " & " << TabFlow1[cn][3][11] << " & " << TabFlow1[cn][5][11] << " & " << TabFlow1[cn][8][11] << " \\\\" << endl;
// end ajout Caro
  ofile << "\\hline " << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;


  if (ChannelName == "mumu")
    ofile << "\\caption{ Dimuon cut flow }" << endl;
  if (ChannelName == "ee")
    ofile << "\\caption{ Dielectron cut flow }" << endl;
  if (ChannelName == "emu")
    ofile << "\\caption{ ElectronMuon cut flow }" << endl;
  ofile << "\\label{Table:CutFlow}" << endl;
  ofile << "\\end{table}" << endl;
 }


  //   //Merge channels consistently
  //   vector<string> mergenames;
  //   mergenames.push_back("TTbarEM");
  //   mergenames.push_back("TTbarMM");
  //   mergenames.push_back("TTbarOther");
  //   selTable_ee.MergeDatasets(mergenames, string("TTbarOther"));
  //   mergenames.clear();
  //   mergenames.push_back("TTbarEE");
  //   mergenames.push_back("TTbarMM");
  //   mergenames.push_back("TTbarOther");
  //   selTable_emu.MergeDatasets(mergenames, string("TTbarOther"));
  //   mergenames.clear();
  //   mergenames.push_back("TTbarEE");
  //   mergenames.push_back("TTbarEM");
  //   mergenames.push_back("TTbarOther");
  //   selTable_mumu.MergeDatasets(mergenames, string("TTbarOther"));
  //   mergenames.clear();
  //   mergenames.push_back("TTbarEE");
  //   mergenames.push_back("TTbarEM");
  //   mergenames.push_back("TTbarMM");
  //   selTable_allChannels.MergeDatasets(mergenames, string("TTbarSignal"));
  //   //Define signal
  //   selTable_ee.DefineFirstDataset(string("TTbarEE"));
  //   selTable_emu.DefineFirstDataset(string("TTbarEM"));
  //   selTable_mumu.DefineFirstDataset(string("TTbarMM"));
  //   selTable_allChannels.DefineFirstDataset(string("TTbarSignal"));
  //   //Calculations
  //   selTable_ee.TableCalculator();
  //   selTable_emu.TableCalculator();
  //   selTable_mumu.TableCalculator();
  //   selTable_allChannels.TableCalculator();
  //   //Write
  //   selTable_ee.Write(ofile);
  //   selTable_emu.Write(ofile);
  //   selTable_mumu.Write(ofile);
  //   selTable_allChannels.Write(ofile);
  ofile << "\\end{document}" << endl;
  system ("pdflatex CrossSectionTable_8.tex");


  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Write output root file " << endl;
    cout << "#########################" << endl;
  }
  string ofilename;
  if(argc>2) ofilename = argv[2];
  else ofilename = string("BtaggingFiles.root"); 
  TFile *fout = new TFile (ofilename.c_str(), "RECREATE");
  //  histoManager.Write(fout);
  //  MyhistoManager.SimpleWrite();
  //modifdg
  //   MyhistoManager.WriteMyHisto(MyHistos,"all");
  //   MyhistoManager.WriteMyHisto(MyHistos1,"all");
  //ChannelName "emu"
  MyhistoManager.WriteMyHisto (HInvM_ll_pair, string("all"));
  MyhistoManager.WriteMyHisto (HInvM_emu_pair, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_JES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_JES_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_JER_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_JER_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_MES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_MES_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_EES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_EES_plus, string("all"));
  MyhistoManager.WriteMyHisto (HMet_emu, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_NBjets, string("all"));
  MyhistoManager.WriteMyHisto (HCount_emu, string("all"));
  MyhistoManager.WriteMyHisto (HCount_emu_after1btag, string("all"));
  MyhistoManager.WriteMyHisto (Hemu_Njets_after1btag, string("all"));
  //SFb
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfb_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfb_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfb_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfb_3b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfb_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfb_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfb_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfb_3b, string("all"));
  //SFl
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfl_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfl_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfl_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_emu_sfl_3b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfl_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfl_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfl_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_emu_sfl_3b, string("all"));
  //
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_JES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_JES_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_JER_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_JER_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_MES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_MES_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_EES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hemu_NjetsNBjets_EES_plus, string("all"));

  //ChannelName "mumu"
  MyhistoManager.WriteMyHisto (HInvM_mumu_pair, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_JES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_JES_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_JER_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_JER_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_MES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_MES_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_EES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_EES_plus, string("all"));
  MyhistoManager.WriteMyHisto (HMet_mumu, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_NBjets, string("all"));
  MyhistoManager.WriteMyHisto (HCount_mumu, string("all"));
  MyhistoManager.WriteMyHisto (HCount_mumu_after1btag, string("all"));
  MyhistoManager.WriteMyHisto (Hmumu_Njets_after1btag, string("all"));
  //SFb
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfb_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfb_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfb_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfb_3b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfb_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfb_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfb_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfb_3b, string("all"));
  //SFl
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfl_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfl_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfl_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_mumu_sfl_3b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfl_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfl_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfl_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_mumu_sfl_3b, string("all"));
  //
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_JES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_JES_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_JER_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_JER_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_MES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_MES_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_EES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hmumu_NjetsNBjets_EES_plus, string("all"));

  // ChannelName "ee" 
  MyhistoManager.WriteMyHisto (HInvM_ee_pair, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_JES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_JES_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_JER_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_JER_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_MES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_MES_plus, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_EES_minus, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_EES_plus, string("all"));
  MyhistoManager.WriteMyHisto (HMet_ee, string("all"));
  MyhistoManager.WriteMyHisto (Hee_NBjets, string("all"));
  MyhistoManager.WriteMyHisto (HCount_ee, string("all"));
  MyhistoManager.WriteMyHisto (HCount_ee_after1btag, string("all"));
  MyhistoManager.WriteMyHisto (Hee_Njets_after1btag, string("all"));
  //SFb
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfb_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfb_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfb_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfb_3b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfb_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfb_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfb_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfb_3b, string("all"));
  //SFl
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfl_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfl_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfl_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HCount2D_ee_sfl_3b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfl_0b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfl_1b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfl_2b, string("all"));
  MyhistoManager.WriteMyHisto2D (HWeight2D_ee_sfl_3b, string("all"));
  //
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_JES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_JES_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_JER_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_JER_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_MES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_MES_plus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_EES_minus, string("all"));
  MyhistoManager.WriteMyHisto2D (Hee_NjetsNBjets_EES_plus, string("all"));


  hS_HInvM_ll_pair->Write ();

  hS_HInvM_emu_pair->Write ();
  hS_Hemu_Njets->Write ();
  hS_HMet_emu->Write ();
  hS_Hemu_NBjets->Write ();

  hS_HInvM_mumu_pair->Write ();
  hS_Hmumu_Njets->Write ();
  hS_HMet_mumu->Write ();
  hS_Hmumu_NBjets->Write ();

  hS_HInvM_ee_pair->Write ();
  hS_Hee_Njets->Write ();
  hS_HMet_ee->Write ();
  hS_Hee_NBjets->Write ();

//DYestimate
  halllistemu->Write ();
  halllistSR->Write ();
  halllistCR->Write ();
  hDataSR->Write ();
  hDataCR->Write ();
  hemudata->Write ();
  NjetsDataCR->Write ();
  NjetsDataSR->Write ();
  NjetsMCCR->Write ();
  NjetsMCSR->Write ();
  for (unsigned int i = 0; i < hlistemu.size (); i++)
    hlistemu[i]->Write ();
  for (unsigned int i = 0; i < hlistSR.size (); i++)
    hlistSR[i]->Write ();
  for (unsigned int i = 0; i < hlistCR.size (); i++)
    hlistCR[i]->Write ();

  //endmodifdg
  //fout->Write();
  fout->Close ();

  //Clear histos before deleting the TFile
  //  histoManager.Clear();  
  //  MyhistoManager.Clear();  

  delete fout;

  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << "    End of the program   " << endl;
    cout << "#########################" << endl;
  }

  return (0);
}
