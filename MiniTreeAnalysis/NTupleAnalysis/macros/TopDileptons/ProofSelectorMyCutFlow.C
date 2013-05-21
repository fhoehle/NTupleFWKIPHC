#define ProofSelectorMyCutFlow_cxx

//////////////////////////////////////////////////////////
//
// Example of TSelector implementation to do a Monte Carlo
// generation using Pythia8.
// See tutorials/proof/runProof.C, option "pythia8", for an
// example of how to run this selector.
//
//////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TFrame.h>
#include <TPaveText.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1F.h>
#include <TMath.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TParameter.h>
#include "TClonesArray.h"
#include "TParticle.h"
#include "TDatabasePDG.h"

#include "ProofSelectorMyCutFlow.h"

//_____________________________________________________________________________
ProofSelectorMyCutFlow::ProofSelectorMyCutFlow()
{
  // Constructor
  
  fChain     = 0;
  branch     = 0;
  event      = 0;
  dataset    = 0;
  anaEL      = 0;
  verbosity  = 0;
  DataType   = 0;
  Luminosity = 0;
  //histos
  fHist      = 0;
  SFtrigger  = 0;  
  SFDY       = 0;   
  SFWjets    = 0; 
  SFQCD      = 0; 
  SFLeptons  = 0;   
  
  
  applyDYscale = true ;
  applyFakescale = true  ;
  
  producePLRPlots       = true; 
  IReweight		= true;
  IDYestimateWithMetCut = true;
  IReweight_puUp	= false;
  IReweight_puDown	= false;
  
  
  doLinearity 	= false;
  
  
  
  /*initMCevents["TTbar"]	        = 3701947;
  initMCevents["DYToLL_M10-50"] = 31480628;
  initMCevents["Zjets"]	        = 36277961;	
  initMCevents["Wjets"]	        = 81352581;	
  initMCevents["TtW"]	        = 814390;  	
  initMCevents["TbartW"]        = 809984;	
  initMCevents["WZ"]	        = 4265243;		
  initMCevents["ZZ"]	        = 4187885;		
  initMCevents["WW"]	        = 4221676;*/
  
  initMCevents["TTbar"]	        = 3631452;
  initMCevents["TTbarScaleUp"]    = 912748;
  initMCevents["TTbarScaleDown"]  = 948880;
  initMCevents["TTbarMatchUp"]    = 1043119;
  initMCevents["TTbarMatchDown"]  = 1043771;
  
  initMCevents["TTbarMass161"]  = 1589234;
  initMCevents["TTbarMass163"]  = 1601897;
  initMCevents["TTbarMass166"]  = 1637204;
  initMCevents["TTbarMass169"]  = 1575820;
  initMCevents["TTbarMass175"]  = 1509012;
  initMCevents["TTbarMass178"]  = 1617587;
  initMCevents["TTbarMass181"]  = 1634464;
  initMCevents["TTbarMass184"]  = 1640594;
  
  
  
  initMCevents["DYToLL_M10-50"] = 31480628; // from DAS
  initMCevents["Zjets"]	        = 36050575;	
  initMCevents["Wjets"]	        = 81352581;	
  initMCevents["TtW"]	        = 812544;  	
  initMCevents["TbartW"]        = 808154;
  initMCevents["TtWScaleUp"]    = 436624;
  initMCevents["TtWScaleDown"]  = 436971;
  initMCevents["TtWMatchingUp"]       = 0;
  initMCevents["TtWMatchingDown"]     = 0;
  initMCevents["TbartWScaleUp"]       = 436714;
  initMCevents["TbartWScaleDown"]     = 436991;
  initMCevents["TbartWMatchingUp"]    = 0;
  initMCevents["TbartWMatchingDown"]  = 0;	   
  initMCevents["WZ"]	        = 4265239;		
  initMCevents["ZZ"]	        = 4187882;		
  initMCevents["WW"]	        = 4221674;
  
  	 
  /*skimMCevent["TTbar"]        = ; 
  skimMCevent["DYToLL_M10-50"]  = ; 
  skimMCevent["Zjets"]	        = ; 
  skimMCevent["Wjets"]	        = ; 
  skimMCevent["TtW"]	        = ; 
  skimMCevent["TbartW"]         = ; 
  skimMCevent["WZ"]	        = ;            
  skimMCevent["ZZ"]	        = ;            
  skimMCevent["WW"]	        = ;*/




}

//_____________________________________________________________________________
ProofSelectorMyCutFlow::~ProofSelectorMyCutFlow()
{
  // Destructor
  
  //SafeDelete(fHist);
}

//_____________________________________________________________________________
void ProofSelectorMyCutFlow::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses of the tree
  // will be set. It is normaly not necessary to make changes to the
  // generated code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running with PROOF.
  
  //fChain point to the loading tree 
  fChain = tree;
  cout << "start init tree " << endl;
  // Set branch addresses
  branch = (TBranch *) tree->GetBranch("NTEvent");
  event = new TopTree::NTEvent();
   branch->SetAddress(&event);
   //event is now retrieved and could be used in Process
   cout << "end init tree " << endl;
}

//_____________________________________________________________________________
void ProofSelectorMyCutFlow::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  cout << "start Begin " << endl;
  TString option = GetOption();
  cout << "end  Begin" << endl;
  
  
}

//_____________________________________________________________________________
void ProofSelectorMyCutFlow::SlaveBegin(TTree * tree)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  
  cout << "start SlaveBegin " << endl;
  TString option = GetOption();
  //--------------------------------------//
  //       Loading the xml file
  //--------------------------------------//
  TNamed *dsname = (TNamed *) fInput->FindObject("PROOF_DATASETNAME"); 
  datasetName = dsname->GetTitle();
  cout << "dataset name " << datasetName << endl;
  cout << "dataset name " << datasetName << endl;
  cout << "dataset name " << datasetName << endl;
  cout << "dataset name " << datasetName << endl;
  cout << "dataset name " << datasetName << endl;
  cout << "dataset name " << datasetName << endl;
  TNamed *xfname = (TNamed *) fInput->FindObject("PROOF_XMLFILENAME");
  string xmlFileName = xfname->GetTitle();
  //gProof->GetManager()->GetFile(xmlFileName.c_str(),"local.xml");
   //anaEL = new AnalysisEnvironmentLoader("/tmp/local.xml");
  anaEL = new AnalysisEnvironmentLoader(xmlFileName.c_str());
  anaEL->LoadSamples (datasets, datasetName); // now the list of datasets written in the xml file is known
  
  
  
  //retrieve the current dataset according to its name
   for(unsigned int d=0;d<datasets.size();d++){
     cout << "datasets.size() " << datasets.size()<< "  datasets[d].Name()" << datasets[d].Name()  << " datasetName "
	  <<datasetName  << endl;
     if(datasets[d].Name()==datasetName)dataset = &datasets[d];
   }
   cout << "load datasets "  << endl;
   anaEL->LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
   cout << "datasets loaded "  << endl;
   //anaEL->LoadGeneralInfo(DataType, Luminosity, verbosity );
   anaEL->LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
   anaEL->LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)

   //Load for PU:
   sel.GeneratePUWeight(PUWeightFileName);
   
   //******************************************
   //Load Scale Factors for lepton efficiencies
   //********d**********************************
   sel.LoadElScaleFactors();
   sel.LoadMuScaleFactors();
   
   //--------------------------------------//
   //   Fill cuts and channels  	
   //--------------------------------------//
   CutName.push_back("Cut1");
   VecChannelName.push_back("ee");
   VecChannelName.push_back("emu");
   VecChannelName.push_back("mumu");
   
   
   //--------------------------------------//
   //   Initializing variables 	
   //--------------------------------------//
   
   
   
   
   
  
   /*    
   SF_trig_mu = 0.969; 
   SF_trig_emu= 1.002; 
   SF_trig_ee = 0.955; 
   */   
   /*SF_trig_mu = 0.969; 
   SF_trig_emu= 1.002; 
   SF_trig_ee = 0.955; */
   
   SF_trig_mu = 0.977; 
   SF_trig_emu= 1.008; 
   SF_trig_ee = 0.962; 

   SF_trig_mu_error  = 0.014; 
   SF_trig_emu_error = 0.007; 
   SF_trig_ee_error  = 0.016; 

   SF_e  =1.0; 
   SF_mu =1.0; 
   
   SF_met_mumu = 1.00237;
   SF_met_emu  = 1.0;
   SF_met_ee   = 1.00237;
   
   
   SF_met_mumu_error = 0.00629333;
   SF_met_emu_error  = 0;
   SF_met_ee_error   = 0.00629333;
  
  
   //**************************************
   //**************************************
   //******* DY DD estimate ***************
   //**************************************
   //**************************************
   
   
   // for PLR plot
   SF_DY_ee = 1.;
   SF_DY_mm = 1.;
   SF_DY_em = 1.;
   
   if(applyDYscale){ 
   
   
   //after dilept invMass
   vSF_DY_ee.push_back(1) ;
   vSF_DY_ee_error.push_back(0) ;
   //after jet cut
   vSF_DY_ee.push_back(1.169) ;
   vSF_DY_ee_error.push_back(0.077) ;
   //after met cut
   vSF_DY_ee.push_back( 1.9281/vSF_DY_ee[1]) ;
   vSF_DY_ee_error.push_back(0.404145) ;
   //for 1 btag 
   vSF_DY_ee.push_back(2.11816/1.9281) ;
   vSF_DY_ee_error.push_back(0.53244) ;
   //for E btag 
   vSF_DY_ee.push_back(2.1314/1.9281) ;
   vSF_DY_ee_error.push_back(1.06501) ;
   
   //after dilept invMass
   vSF_DY_mm.push_back(1) ;
   vSF_DY_mm_error.push_back(0) ;
   //after jet cut
   vSF_DY_mm.push_back(1.257) ;
   vSF_DY_mm_error.push_back(0.078) ;
   //after met cut
   vSF_DY_mm.push_back(1.82219/vSF_DY_mm[1]) ;
   vSF_DY_mm_error.push_back(0.365828) ;
   //for 1 btag 
   vSF_DY_mm.push_back(1.73089/1.82219) ;
   vSF_DY_mm_error.push_back(0.428215) ;
   //for E btag 
   vSF_DY_mm.push_back(0.819327/1.82219) ;
   vSF_DY_mm_error.push_back(0.667288) ;




   //after dilept invMass
   vSF_DY_em.push_back(1) ;
   vSF_DY_em_error.push_back(0) ;
   //after jet cut
   vSF_DY_em.push_back(1.38872) ;
   vSF_DY_em_error.push_back(0.296232) ;
   //after met cut
   vSF_DY_em.push_back(1) ;
   vSF_DY_em_error.push_back(0.296232) ;
   //for 1 btag 
   vSF_DY_em.push_back(1) ;
   vSF_DY_em_error.push_back(0.296232) ;
   //for E btag 
   vSF_DY_em.push_back(1) ;
   vSF_DY_em_error.push_back(0.296232) ;
   
   
   // for PLR plot
   SF_DY_ee = 1.9281;
   SF_DY_mm = 1.82219;
   SF_DY_em = 1.38872;
   
   
   }else{
   
   
   for(unsigned int i=0; i< 5; i++){
     vSF_DY_em.push_back(1.) ;
     vSF_DY_em_error.push_back(0.) ;
     vSF_DY_ee.push_back(1.) ;
     vSF_DY_ee_error.push_back(0.) ;
     vSF_DY_mm.push_back(1.) ;
     vSF_DY_mm_error.push_back(0.) ;
   }
   
   
   
   }
   SF_BranchingRatio_ll = (0.108*9.)*(0.108*9.);
   SF_BranchingRatio_lj = (0.108*9.)*(0.676*1.5);
   SF_BranchingRatio_had = (0.676*1.5)*(0.676*1.5);
   
   
   
   
   
   //**************************************
   //**************************************
   //******* fakes DD estimate ************
   //**************************************
   //**************************************
   
   
   if(applyFakescale){  
   //after MET selection
   vSF_FakeBck_ee.push_back(0.966); 
   vSF_FakeBck_ee_error.push_back(0.674); 
   //after 1 b jet
   vSF_FakeBck_ee.push_back(0.35/vSF_FakeBck_ee[0]); 
   vSF_FakeBck_ee_error.push_back(0.615); 
   //after 2 b jet
   vSF_FakeBck_ee.push_back(0.35/vSF_FakeBck_ee[0]); 
   vSF_FakeBck_ee_error.push_back(0.615); 
   
   
   
   //after MET selection
   vSF_FakeBck_mm.push_back(4.9); 
   vSF_FakeBck_mm_error.push_back(2.3); 
   //after 1 b jet
   vSF_FakeBck_mm.push_back(4.0/vSF_FakeBck_mm[0]); 
   vSF_FakeBck_mm_error.push_back(2.2); 
   //after 2 b jet
   vSF_FakeBck_mm.push_back(4.0/vSF_FakeBck_mm[0]); 
   vSF_FakeBck_mm_error.push_back(2.2);
   
   
    
   //after MET selection
   vSF_FakeBck_em.push_back(3.8); 
   vSF_FakeBck_em_error.push_back(0.9); 
   //after 1 b jet
   vSF_FakeBck_em.push_back(2.5/vSF_FakeBck_em[0]); 
   vSF_FakeBck_em_error.push_back(0.25); 
   //after 2 b jet
   vSF_FakeBck_em.push_back(2.5/vSF_FakeBck_em[0]); 
   vSF_FakeBck_em_error.push_back(0.25); 
   
    
   
   SF_Wjets_ee = 0.966 ;
   SF_Wjets_mm = 4.9 ;
   SF_Wjets_em = 3.8;
   
   SF_QCD_ee = 1.;
   SF_QCD_mm = 1.;
   SF_QCD_em = 1.;
   
   }else{
   
   
    for(unsigned int i=0; i< 5; i++){
      vSF_FakeBck_em.push_back(1.) ;
      vSF_FakeBck_em_error.push_back(0.) ;
      vSF_FakeBck_ee.push_back(1.) ;
      vSF_FakeBck_ee_error.push_back(0.) ;
      vSF_FakeBck_mm.push_back(1.) ;
      vSF_FakeBck_mm_error.push_back(0.) ;
    }
   }
   
		  
   sumSFlept_ee    = 0;
   sumSFlept_mumu  = 0;
   sumSFlept_emu   = 0;
   
   nEvents_ee   = 0;
   nEvents_mumu   = 0;
   nEvents_emu   = 0;
   
   
   scaleElec = 1.0; // 1 to switch off
   resolElec = 0.0; // 0 to switch off
   
   ApplyLeptonSF = true;
   
   ITypeMC     = -1;
   ICut        = -1;  
   
   
   //************************************
   //For trigger systematics 
   
   if(datasetName=="TTbarTriggerUp"){
     SF_trig_mu += SF_trig_mu_error;
     SF_trig_emu+= SF_trig_emu_error;  
     SF_trig_ee += SF_trig_ee_error; 
   } 
   if(datasetName=="TTbarTriggerDown"){
     SF_trig_mu  -= SF_trig_mu_error;
     SF_trig_emu -= SF_trig_emu_error;  
     SF_trig_ee  -= SF_trig_ee_error; 
   } 
   
   //************************************
   
   //************************************
   //For MET systematics 
   
   
   if(datasetName=="TTbarMETUp"){
     SF_met_mumu  += SF_met_mumu_error;
     SF_met_emu += SF_met_emu_error;  
     SF_met_ee  += SF_met_ee_error; 
   } 
   if(datasetName=="TTbarMETDown"){
     SF_met_mumu  -= SF_met_mumu_error;
     SF_met_emu -= SF_met_emu_error;  
     SF_met_ee  -= SF_met_ee_error; 
   } 
   
   if(datasetName=="TTbarPUup")   IReweight_puUp	= true;
   if(datasetName=="TTbarPUdown") IReweight_puDown	= true;
   
    if ( datasetName=="TTbar" || 
         datasetName=="TTbarScaleUp" ||
         datasetName=="TTbarScaleDown" ||
         datasetName=="TTbarMatchUp" ||
         datasetName=="TTbarMatchDown" ||
         datasetName=="TTbarMass161" ||
         datasetName=="TTbarMass163" ||
         datasetName=="TTbarMass166" ||
         datasetName=="TTbarMass169" ||
         datasetName=="TTbarMass175" ||
         datasetName=="TTbarMass178" ||
         datasetName=="TTbarMass181" ||
         datasetName=="TTbarMass184" ||
         datasetName=="TTbarPUup" ||
         datasetName=="TTbarPUdown" ||
         datasetName=="TTbarPDFup" ||
         datasetName=="TTbarPDFdown" ||
         datasetName=="TTbarLeptUp" ||
         datasetName=="TTbarLeptDown" ||
         datasetName=="TTbarTriggerUp" ||
         datasetName=="TTbarTriggerDown" ||
         datasetName=="TTbarMETUp" ||
         datasetName=="TTbarMETDown" 
    
    
	 ) { 
      datasetName="TTbar";
      }
      
      
   for(unsigned int d=0;d<datasets.size();d++){
     cout << "datasets.size() " << datasets.size()<< "  datasets[d].Name()" << datasets[d].Name()  << " datasetName "
	  <<datasetName  << endl;
     if(datasets[d].Name()==datasetName)dataset = &datasets[d];
   }
   plotsFurnisher.LoadSetup(*anaEL,datasets,CutName,VecChannelName);
   if(plotsFurnisher.GetListOfSystematics().size()==0) producePLRPlots = false;
   //producePLRPlots= false;
   if(producePLRPlots) plotsFurnisher.CreateHistos();
   //producePLRPlots= false;
   
   
   
   
   //--------------------------------------//
   //   Managing histos  	
   //--------------------------------------//
   cout << "load datasets in histo "  << endl;
   MyhistoManager.LoadDatasets(datasets);   
   cout << "datasets loaded"  << endl;
   MyhistoManager.LoadSelectionSteps(CutName);
   MyhistoManager.LoadChannels(ChannelName);
   //example
   fHist = new TH1F("fHist", "jet pt", 8, -0.5, 7.5);
   
   nbins = 200;
   minx = 0.;
   maxx = 350;
   
   
   //***********************
   // initiate lumi reweighting
   //***********************
   
   PShiftDown_ = reweight::PoissonMeanShifter(-0.6);
   PShiftUp_   = reweight::PoissonMeanShifter(0.6);

   cout << "datasetName 2 " << datasetName << endl;
   cout << "datasetName 2 " << datasetName << endl;
   cout << "datasetName 2 " << datasetName << endl;
   cout << "datasetName 2 " << datasetName << endl;
   cout << "datasetName 2 " << datasetName << endl;
   cout << "datasetName 2 " << datasetName << endl;
   
   cout << "dataset name for histograms " << datasetName << endl;
   
   
   
      
   MyhistoManager.CreateHisto(CutFlow_mumu,  "CutFlow_mumu" ,datasetName,"CutFlow","Entries",15,-0.5,14.5);
   MyhistoManager.CreateHisto(CutFlow_ee,    "CutFlow_ee"   ,datasetName,"CutFlow","Entries",15,-0.5,14.5);
   MyhistoManager.CreateHisto(CutFlow_emu,   "CutFlow_emu"  ,datasetName,"CutFlow","Entries",15,-0.5,14.5);
   
   
   MyhistoManager.SetCutFlowAxisTitle(CutFlow_mumu,  "CutFlow_mumu" ,datasetName);
   MyhistoManager.SetCutFlowAxisTitle(CutFlow_ee,    "CutFlow_ee"   ,datasetName);
   MyhistoManager.SetCutFlowAxisTitle(CutFlow_emu,   "CutFlow_emu"  ,datasetName);
   
   
   MyhistoManager.CreateHisto(ErrCutFlow_mumu,  "ErrCutFlow_mumu" ,datasetName,"ErrCutFlow","Entries",15,-0.5,14.5);
   MyhistoManager.CreateHisto(ErrCutFlow_ee,    "ErrCutFlow_ee"   ,datasetName,"ErrCutFlow","Entries",15,-0.5,14.5);
   MyhistoManager.CreateHisto(ErrCutFlow_emu,   "ErrCutFlow_emu"  ,datasetName,"ErrCutFlow","Entries",15,-0.5,14.5);
   
   SFtrigger = new TH1D("SFtrigger","SFtrigger", 20,-0.5,19.5);
   
   SFDY      = new TH1D("SFDY",     "SFDY"     , 20,-0.5,19.5);   
   SFWjets   = new TH1D("SFWjets",  "SFWjets"  , 20,-0.5,19.5);  
   SFQCD     = new TH1D("SFQCD",    "SFQCD"    , 20,-0.5,19.5); 
     
   SFLeptons = new TH1D("SFLeptons","SFLeptons", 20,-0.5,19.5);  
   
   
   
   MyhistoManager.CreateHisto(LeptonIso_e,"LeptonIso_e" ,datasetName,"Liso","Entries",100,0.,1.);
   MyhistoManager.CreateHisto(LeptonIso_mu,"LeptonIso_mu" ,datasetName,"Liso","Entries",100,0.,1.);
   
   
   MyhistoManager.CreateHisto(MyHistos_ee,                   "InvDilMassNJinf1_ee" ,datasetName,"Mll","Entries",50,0.,350);
   
   MyhistoManager.CreateHisto(MyHistos1_ee,                  "InvDilMassNJsup1_ee" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_pair_ee,              "InvDilMassPair_ee" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(MyHistos_mumu,                   "InvDilMassNJinf1_mumu" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(MyHistos1_mumu,                  "InvDilMassNJsup1_mumu" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_pair_mumu,              "InvDilMassPair_mumu" ,datasetName,"Mll","Entries",50,0.,350);
   
   MyhistoManager.CreateHisto(HInvM_ll_pair_ee_sup15,          "InvDilMassPair_ee_sup15" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_pair_mumu_inf15,        "InvDilMassPair_mumu_inf15" ,datasetName,"Mll","Entries",50,0.,350);
   
   MyhistoManager.CreateHisto(HHT_ee_aftermetbtag1,   "HHT_ee_aftermetbtag1"  , datasetName,"HT","Entries",1000,0.,1000);
   MyhistoManager.CreateHisto(HHT_mumu_aftermetbtag1, "HHT_mumu_aftermetbtag1", datasetName,"HT","Entries",1000,0.,1000);
   MyhistoManager.CreateHisto(HHT_emu_aftermetbtag1,  "HHT_emu_aftermetbtag1" , datasetName,"HT","Entries",1000,0.,1000);
   
   MyhistoManager.CreateHisto(HHT_ee_aftermetbtag2,   "HHT_ee_aftermetbtag2"  , datasetName,"HT","Entries",1000,0.,1000);
   MyhistoManager.CreateHisto(HHT_mumu_aftermetbtag2, "HHT_mumu_aftermetbtag2", datasetName,"HT","Entries",1000,0.,1000);
   MyhistoManager.CreateHisto(HHT_emu_aftermetbtag2,  "HHT_emu_aftermetbtag2" , datasetName,"HT","Entries",1000,0.,1000);
   
   MyhistoManager.CreateHisto(HInvM_emu_pair,             "InvemuMassPair" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_emu_pairzoom,             "InvemuMassPairzoom" ,datasetName,"Mll","Entries",30,60.,120.);
   MyhistoManager.CreateHisto(HInvM_emu_pair_afterbtag1, "InvemuMassPair_afterbtag1" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_emu_pair_afterbtag2, "InvemuMassPair_afterbtag2" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_emu_pair_aftermetbtag1, "InvemuMassPair_aftermetbtag1" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_emu_pair_aftermetbtag2, "InvemuMassPair_aftermetbtag2" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_emu,              "Njets_emu" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   
   MyhistoManager.CreateHisto(HNjets_ee_aftermetcutbutjetcut,  "Njets_ee_aftermetcutbutjetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HNjets_mumu_aftermetcutbutjetcut, "Njets_mumu_aftermetcutbutjetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HNjets_emu_aftermetcutbutjetcut, "Njets_emu_aftermetcutbutjetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);


   MyhistoManager.CreateHisto(HMet_emu,                "Met_emu" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_emu,             "NBjets_emu" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtLpt1_emu,		"PtLpt1_emu" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtLpt2_emu,		"PtLpt2_emu" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_emu,		"PtJet1_emu" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_emu,		"PtJet2_emu" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HNVxP_emu,		"NVxP_emu" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   MyhistoManager.CreateHisto(HNVxP_emu_aftertrigger,   "NVxP_emu_aftertrigger" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   MyhistoManager.CreateHisto(HInvM_emu_pair_Zemu, "InvemuMassPair_Zemu" ,datasetName,"Mll","Entries",50,0.,350);
   
   MyhistoManager.CreateHisto(HInvM_emu_pair_aftermetcut, "InvemuMassPair_aftermetcut" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_emu_aftermetcut,  "Njets_emu_aftermetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_emu_aftermetcut,    "Met_emu_aftermetcut" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_emu_aftermetcut, "NBjets_emu_aftermetcut" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtLpt1_emu_aftermetcut, "PtLpt1_emu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtLpt2_emu_aftermetcut, "PtLpt2_emu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_emu_aftermetcut, "PtJet1_emu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_emu_aftermetcut, "PtJet2_emu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HNVxP_emu_aftermetcut,   "NVxP_emu_aftermetcut" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   
   MyhistoManager.CreateHisto(HInvM_emu_pair_afterjetcut, "InvemuMassPair_afterjetcut" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_emu_afterjetcut,  "Njets_emu_afterjetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_emu_afterjetcut,    "Met_emu_afterjetcut" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_emu_afterjetcut, "NBjets_emu_afterjetcut" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtLpt1_emu_afterjetcut, "PtLpt1_emu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtLpt2_emu_afterjetcut, "PtLpt2_emu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_emu_afterjetcut, "PtJet1_emu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_emu_afterjetcut, "PtJet2_emu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   
   MyhistoManager.CreateHisto(HDeltaPhiLMet_emu,    "DeltaPhiLMet_emu" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_emu_aftermetcut,    "DeltaPhiLMet_emu_aftermetcut" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_emu_afterjetcut,    "DeltaPhiLMet_emu_afterjetcut" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   
   MyhistoManager.CreateHisto(HInvM_mumu_pair,             "InvmumuMassPair" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_mumu_pairzoom,         "InvmumuMassPairzoom" ,datasetName,"Mll","Entries",30,60.,120.);
   MyhistoManager.CreateHisto(HInvM_mumu_pair_afterbtag1, "InvmumuMassPair_afterbtag1" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_mumu_pair_afterbtag2, "InvmumuMassPair_afterbtag2" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_mumu_pair_aftermetbtag1, "InvmumuMassPair_aftermetbtag1" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_mumu_pair_aftermetbtag2, "InvmumuMassPair_aftermetbtag2" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_mumu,              "Njets_mumu" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_mumu,                "Met_mumu" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_mumu,             "NBjets_mumu" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtMu1_mumu,              "PtMu1_mumu" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtMu2_mumu,              "PtMu2_mumu" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_mumu,             "PtJet1_mumu" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_mumu,             "PtJet2_mumu" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HNVxP_mumu,		"NVxP_mumu" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   MyhistoManager.CreateHisto(HNVxP_mumu_aftertrigger,   "NVxP_mumu_aftertrigger" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   
   MyhistoManager.CreateHisto(HInvM_mumu_pair_aftermetcut, "InvmumuMassPair_aftermetcut" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_mumu_aftermetcut,  "Njets_mumu_aftermetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_mumu_aftermetcut,    "Met_mumu_aftermetcut" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_mumu_aftermetcut, "NBjets_mumu_aftermetcut" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtMu1_mumu_aftermetcut,  "PtMu1_mumu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtMu2_mumu_aftermetcut,  "PtMu2_mumu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_mumu_aftermetcut, "PtJet1_mumu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_mumu_aftermetcut, "PtJet2_mumu_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HNVxP_mumu_aftermetcut,   "NVxP_mumu_aftermetcut" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   
   MyhistoManager.CreateHisto(HInvM_mumu_pair_afterjetcut, "InvmumuMassPair_afterjetcut" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_mumu_afterjetcut,  "Njets_mumu_afterjetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_mumu_afterjetcut,    "Met_mumu_afterjetcut" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_mumu_afterjetcut, "NBjets_mumu_afterjetcut" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtMu1_mumu_afterjetcut,  "PtMu1_mumu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtMu2_mumu_afterjetcut,  "PtMu2_mumu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_mumu_afterjetcut, "PtJet1_mumu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_mumu_afterjetcut, "PtJet2_mumu_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   
   MyhistoManager.CreateHisto(HInvM_mumu_pair_afterveto, "InvmumuMassPair_afterveto" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_mumu_afterveto,  "Njets_mumu_afterveto" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_mumu_afterveto,    "Met_mumu_afterveto" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_mumu_afterveto, "NBjets_mumu_afterveto" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtMu1_mumu_afterveto,  "PtMu1_mumu_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtMu2_mumu_afterveto,  "PtMu2_mumu_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_mumu_afterveto, "PtJet1_mumu_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_mumu_afterveto, "PtJet2_mumu_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   
   MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu,    "DeltaPhiLMet_mumu" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu_aftermetcut,    "DeltaPhiLMet_mumu_aftermetcut" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu_afterjetcut,    "DeltaPhiLMet_mumu_afterjetcut" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu_afterveto,    "DeltaPhiLMet_mumu_afterveto" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   
   MyhistoManager.CreateHisto(HInvM_ee_pair,             "InveeMassPair" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_ee_pairzoom,         "InveeMassPairzoom" ,datasetName,"Mll","Entries",30,60.,120.);
   MyhistoManager.CreateHisto(HInvM_ee_pair_afterbtag1, "InveeMassPair_afterbtag1" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_ee_pair_afterbtag2, "InveeMassPair_afterbtag2" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_ee_pair_aftermetbtag1, "InveeMassPair_aftermetbtag1" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HInvM_ee_pair_aftermetbtag2, "InveeMassPair_aftermetbtag2" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_ee,              "Njets_ee" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_ee,                "Met_ee" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_ee,             "NBjets_ee" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtEl1_ee,              "PtEl1_ee" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtEl2_ee,              "PtEl2_ee" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_ee,             "PtJet1_ee" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_ee,             "PtJet2_ee" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HNVxP_ee,		"NVxP_ee" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   MyhistoManager.CreateHisto(HNVxP_ee_aftertrigger,   "NVxP_ee_aftertrigger" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   
   MyhistoManager.CreateHisto(HInvM_ee_pair_aftermetcut, "InveeMassPair_aftermetcut" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_ee_aftermetcut,  "Njets_ee_aftermetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_ee_aftermetcut,    "Met_ee_aftermetcut" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_ee_aftermetcut, "NBjets_ee_aftermetcut" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtEl1_ee_aftermetcut,  "PtEl1_ee_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtEl2_ee_aftermetcut,  "PtEl2_ee_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_ee_aftermetcut, "PtJet1_ee_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_ee_aftermetcut, "PtJet2_ee_aftermetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HNVxP_ee_aftermetcut,   "NVxP_ee_aftermetcut" ,datasetName,"NbVxP","Entries",20,-0.5,19.5);
   
   MyhistoManager.CreateHisto(HInvM_ee_pair_afterjetcut, "InveeMassPair_afterjetcut" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_ee_afterjetcut,  "Njets_ee_afterjetcut" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_ee_afterjetcut,    "Met_ee_afterjetcut" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_ee_afterjetcut, "NBjets_ee_afterjetcut" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtEl1_ee_afterjetcut,  "PtEl1_ee_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtEl2_ee_afterjetcut,  "PtEl2_ee_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_ee_afterjetcut, "PtJet1_ee_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_ee_afterjetcut, "PtJet2_ee_afterjetcut" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   
   MyhistoManager.CreateHisto(HInvM_ee_pair_afterveto, "InveeMassPair_afterveto" ,datasetName,"Mll","Entries",50,0.,350);
   MyhistoManager.CreateHisto(HNjets_ee_afterveto,  "Njets_ee_afterveto" ,datasetName,"Njets","Entries",5,-0.5,4.5);
   MyhistoManager.CreateHisto(HMet_ee_afterveto,    "Met_ee_afterveto" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HNBjets_ee_afterveto, "NBjets_ee_afterveto" ,datasetName,"NBjets","Entries",4,-0.5,3.5);
   MyhistoManager.CreateHisto(HPtEl1_ee_afterveto,  "PtEl1_ee_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtEl2_ee_afterveto,  "PtEl2_ee_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,250);
   MyhistoManager.CreateHisto(HPtJet1_ee_afterveto, "PtJet1_ee_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   MyhistoManager.CreateHisto(HPtJet2_ee_afterveto, "PtJet2_ee_afterveto" ,datasetName,"Pt(GeV)","Entries",50,0.,300);
   
   MyhistoManager.CreateHisto(HDeltaPhiLMet_ee,    "DeltaPhiLMet_ee" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_ee_aftermetcut,    "DeltaPhiLMet_ee_aftermetcut" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_ee_afterjetcut,    "DeltaPhiLMet_ee_afterjetcut" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   MyhistoManager.CreateHisto(HDeltaPhiLMet_ee_afterveto,    "DeltaPhiLMet_ee_afterveto" ,datasetName,"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
   
   MyhistoManager.CreateHisto(HMet_emu_afterbtag1,    "Met_emu_afterbtag1" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HMet_mumu_afterbtag1,    "Met_mumu_afterbtag1" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HMet_ee_afterbtag1,    "Met_ee_afterbtag1" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HMet_emu_afterbtag2,    "Met_emu_afterbtag2" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HMet_mumu_afterbtag2,    "Met_mumu_afterbtag2" ,datasetName,"Met","Entries",30,0,150);
   MyhistoManager.CreateHisto(HMet_ee_afterbtag2,    "Met_ee_afterbtag2" ,datasetName,"Met","Entries",30,0,150);
   
   MyhistoManager.CreateHisto(pileup,                "pileup"            ,datasetName,"pileup","Entries",36,-0.5,35.5);
   
   
   MyhistoManager.CreateHisto(HInvM_ee_pair_EB,  "HInvM_ee_pair_EB"  ,datasetName,"","Entries", 60, 60 ,120);
   MyhistoManager.CreateHisto(HInvM_ee_pair_EE,  "HInvM_ee_pair_EE"  ,datasetName,"","Entries", 60, 60 ,120);
   MyhistoManager.CreateHisto2D (HInvM_ee_pair_vs_MET_NBJet0,    "HInvM_ee_pair_vs_MET_NBJet0",     datasetName,  "Minv", 350,0.,350, "MET",30,0,150);    
   
   MyhistoManager.CreateHisto2D (HInvM_mumu_pair_vs_MET_NBJet0,    "HInvM_mumu_pair_vs_MET_NBJet0",    datasetName,  "Minv", 350,0.,350, "MET",30,0,150);   
   
   MyhistoManager.CreateHisto2D (HInvM_ee_pair_vs_NJet_METinf10, "HInvM_ee_pair_vs_NJet_METinf10", datasetName,  "Minv", 350,0.,350, "NJet",5, -0.5, 4.5 );
   MyhistoManager.CreateHisto2D (HInvM_ee_pair_vs_MET_NJet0,     "HInvM_ee_pair_vs_MET_NJet0",     datasetName,  "Minv", 350,0.,350, "MET",30,0,150);    
   MyhistoManager.CreateHisto2D (HInvM_ee_pair_vs_PU,            "HInvM_ee_pair_vs_PU",            datasetName,  "Minv", 350,0.,350, "MET",25,0,25);    
   MyhistoManager.CreateHisto3D (HInvM_ee_pair_vs_MET_NJet,      "HInvM_ee_pair_vs_MET_NJet",      datasetName,  "Minv", 350,0.,350, "MET",30,0,150, "NJet",5, -0.5, 4.5);     
   
   MyhistoManager.CreateHisto2D (HInvM_mumu_pair_vs_NJet_METinf10, "HInvM_mumu_pair_vs_NJet_METinf10", datasetName,  "Minv", 350,0.,350, "NJet",5, -0.5, 4.5 );
   MyhistoManager.CreateHisto2D (HInvM_mumu_pair_vs_MET_NJet0,     "HInvM_mumu_pair_vs_MET_NJet0",     datasetName,  "Minv", 350,0.,350, "MET",30,0,150);     
   MyhistoManager.CreateHisto2D (HInvM_mumu_pair_vs_PU,            "HInvM_mumu_pair_vs_PU",            datasetName,  "Minv", 350,0.,350, "MET",25,0,25);    
   MyhistoManager.CreateHisto3D (HInvM_mumu_pair_vs_MET_NJet,      "HInvM_mumu_pair_vs_MET_NJet",      datasetName,  "Minv", 350,0.,350, "MET",30,0,150, "NJet",5, -0.5, 4.5);     
 
   
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_mumu,   "HInvM_ll_afterSelButInvMNJet_AfterMET_mumu",   datasetName,  "Minv", "Entries", 350,0.,350); 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_ee  ,   "HInvM_ll_afterSelButInvMNJet_AfterMET_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350); 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_emu ,   "HInvM_ll_afterSelButInvMNJet_AfterMET_emu",    datasetName,  "Minv", "Entries", 350,0.,350); 
   

 
 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMDilepton_mumu,   "HInvM_ll_afterSelButInvMDilepton_mumu",   datasetName,  "Minv", "Entries", 350,0.,350); 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMDilepton_ee  ,   "HInvM_ll_afterSelButInvMDilepton_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350); 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMDilepton_emu ,   "HInvM_ll_afterSelButInvMDilepton_emu",    datasetName,  "Minv", "Entries", 350,0.,350); 
 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMNJet_mumu,   "HInvM_ll_afterSelButInvMNJet_mumu",   datasetName,  "Minv", "Entries", 350,0.,350); 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMNJet_ee  ,   "HInvM_ll_afterSelButInvMNJet_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350); 
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMNJet_emu ,   "HInvM_ll_afterSelButInvMNJet_emu",    datasetName,  "Minv", "Entries", 350,0.,350); 
   
   
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM0Btag_mumu,  "HInvM_ll_afterSelButInvM0Btag_mumu",   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM0Btag_ee  ,  "HInvM_ll_afterSelButInvM0Btag_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM0Btag_emu  , "HInvM_ll_afterSelButInvM0Btag_emu"  ,  datasetName,  "Minv", "Entries", 350,0.,350);
   
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM1Btag_mumu,  "HInvM_ll_afterSelButInvM1Btag_mumu",   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM1Btag_ee  ,  "HInvM_ll_afterSelButInvM1Btag_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM1Btag_emu  , "HInvM_ll_afterSelButInvM1Btag_emu"  ,  datasetName,  "Minv", "Entries", 350,0.,350);
   
   
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM2Btag_mumu,  "HInvM_ll_afterSelButInvM2Btag_mumu",   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM2Btag_ee  ,  "HInvM_ll_afterSelButInvM2Btag_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvM2Btag_emu  , "HInvM_ll_afterSelButInvM2Btag_emu"  ,  datasetName,  "Minv", "Entries", 350,0.,350);
   
   
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup0Btag_mumu,  "HInvM_ll_afterSelButInvMSup0Btag_mumu",   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup0Btag_ee  ,  "HInvM_ll_afterSelButInvMSup0Btag_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup0Btag_emu  , "HInvM_ll_afterSelButInvMSup0Btag_emu"  ,  datasetName,  "Minv", "Entries", 350,0.,350);
   
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup1Btag_mumu,  "HInvM_ll_afterSelButInvMSup1Btag_mumu",   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup1Btag_ee  ,  "HInvM_ll_afterSelButInvMSup1Btag_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup1Btag_emu  , "HInvM_ll_afterSelButInvMSup1Btag_emu"  ,  datasetName,  "Minv", "Entries", 350,0.,350);
   
   
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup2Btag_mumu,  "HInvM_ll_afterSelButInvMSup2Btag_mumu",   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup2Btag_ee  ,  "HInvM_ll_afterSelButInvMSup2Btag_ee"  ,   datasetName,  "Minv", "Entries", 350,0.,350);
   MyhistoManager.CreateHisto(HInvM_ll_afterSelButInvMSup2Btag_emu  , "HInvM_ll_afterSelButInvMSup2Btag_emu"  ,  datasetName,  "Minv", "Entries", 350,0.,350);
   
   
   MyhistoManager.CreateHisto(HDileptPt_mumu,  "HDileptPt_mumu",   datasetName,  "DileptPt", "Entries",30,0,150 );
   MyhistoManager.CreateHisto(HDileptPt_ee,    "HDileptPt_ee",     datasetName,  "DileptPt", "Entries",30,0,150 );
   MyhistoManager.CreateHisto(HDileptPt_emu,   "HDileptPt_emu",    datasetName,  "DileptPt", "Entries",30,0,150 );

   
   MyhistoManager.CreateHisto(HDeltaPhi_ee_LeptonMET,          "HDeltaPhi_ee_LeptonMET",      datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_ee_DiLeptonMET,        "HDeltaPhi_ee_DiLeptonMET",    datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_ee_DiLeptonJets,       "HDeltaPhi_ee_DiLeptonJets",   datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_ee_ClosestDiLeptonJets,"HDeltaPhi_ee_ClosestDiLeptonJets",   datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_ee_JetsMET,            "HDeltaPhi_ee_JetsMET",        datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_ee_ClosestJetsMET,     "HDeltaPhi_ee_ClosestJetsMET", datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_ee_PtJetsInMassP,      "HDeltaPhi_ee_PtJetsInMassP",  datasetName,  "DeltaPhi", "Entries", 50,0.,200);
   MyhistoManager.CreateHisto(HDeltaPhi_ee_PtJetsOutMassP,     "HDeltaPhi_ee_PtJetsOutMassP", datasetName,  "DeltaPhi", "Entries", 50,0.,200);
   
   MyhistoManager.CreateHisto(HDeltaPhi_emu_LeptonMET,          "HDeltaPhi_emu_LeptonMET",      datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_emu_DiLeptonMET,        "HDeltaPhi_emu_DiLeptonMET",    datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_emu_DiLeptonJets,       "HDeltaPhi_emu_DiLeptonJets",   datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_emu_ClosestDiLeptonJets,"HDeltaPhi_emu_ClosestDiLeptonJets",   datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_emu_JetsMET,            "HDeltaPhi_emu_JetsMET",        datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_emu_ClosestJetsMET,     "HDeltaPhi_emu_ClosestJetsMET", datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_emu_PtJetsInMassP,      "HDeltaPhi_emu_PtJetsInMassP",  datasetName,  "DeltaPhi", "Entries", 50,0.,200);
   MyhistoManager.CreateHisto(HDeltaPhi_emu_PtJetsOutMassP,     "HDeltaPhi_emu_PtJetsOutMassP", datasetName,  "DeltaPhi", "Entries", 50,0.,200);
   
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_LeptonMET,          "HDeltaPhi_mumu_LeptonMET",      datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_DiLeptonMET,        "HDeltaPhi_mumu_DiLeptonMET",    datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_DiLeptonJets,       "HDeltaPhi_mumu_DiLeptonJets",   datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_ClosestDiLeptonJets,"HDeltaPhi_mumu_ClosestDiLeptonJets",   datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_JetsMET,            "HDeltaPhi_mumu_JetsMET",        datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_ClosestJetsMET,     "HDeltaPhi_mumu_ClosestJetsMET", datasetName,  "DeltaPhi", "Entries", 50,0.,3.5);
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_PtJetsInMassP,      "HDeltaPhi_mumu_PtJetsInMassP",  datasetName,  "DeltaPhi", "Entries", 50,0.,200);
   MyhistoManager.CreateHisto(HDeltaPhi_mumu_PtJetsOutMassP,     "HDeltaPhi_mumu_PtJetsOutMassP", datasetName,  "DeltaPhi", "Entries", 50,0.,200);
   
   // Creation of histogram list for DY
   
   MyhistoManager.CreateHisto(hemudata_emu,  "hemudata_emu",   datasetName,  "Minv", "Entries",nbins,minx,maxx );

   MyhistoManager.CreateHisto(hlistemu_ee,  "hlistemu_ee",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(hlistSR_ee,  "hlistSR_ee",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(hlistCR_ee,  "hlistCR_ee",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(hlistContCR_ee,  "hlistContCR_ee",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(hlistemu_mumu,  "hlistemu_mumu",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(hlistSR_mumu,  "hlistSR_mumu",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(hlistCR_mumu,  "hlistCR_mumu",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(hlistContCR_mumu,  "hlistContCR_mumu",   datasetName,  "Minv", "Entries",nbins,minx,maxx );

   MyhistoManager.CreateHisto(NjetsCR_inMZ_mumu,  "NjetsCR_inMZ_mumu",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );
   MyhistoManager.CreateHisto(NjetsSR_inMZ_mumu,  "NjetsSR_inMZ_mumu",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );

   MyhistoManager.CreateHisto(NjetsCR_outMZ_mumu,  "NjetsCR_outMZ_mumu",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );
   MyhistoManager.CreateHisto(NjetsSR_outMZ_mumu,  "NjetsSR_outMZ_mumu",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );

   MyhistoManager.CreateHisto(DYMassCR_mumu,  "DYMassCR_mumu",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(DYMassSR_mumu,  "DYMassSR_mumu",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   
   MyhistoManager.CreateHisto(NjetsCR_inMZ_ee,  "NjetsCR_inMZ_ee",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );
   MyhistoManager.CreateHisto(NjetsSR_inMZ_ee,  "NjetsSR_inMZ_ee",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );

   MyhistoManager.CreateHisto(NjetsCR_outMZ_ee,  "NjetsCR_outMZ_ee",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );
   MyhistoManager.CreateHisto(NjetsSR_outMZ_ee,  "NjetsSR_outMZ_ee",   datasetName,  "Njets", "Entries",5,-0.5,4.5 );

   MyhistoManager.CreateHisto(DYMassCR_ee,  "DYMassCR_ee",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   MyhistoManager.CreateHisto(DYMassSR_ee,  "DYMassSR_ee",   datasetName,  "Minv", "Entries",nbins,minx,maxx );
   
   //--------------------------------------//
   //   pileup reweighting  	
   //--------------------------------------//
   
   
  
   //gSystem->Load("libNTuple.so");
   if (IReweight ) {
   
     if(datasetName == "DYToLL_M10-50"){
       string datafile = "/opt/sbg/data/data1/cms/jandrea/TopIPHC_2012_01_25/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/PUdata.root";
       string mcfile   = "/opt/sbg/data/data1/cms/jandrea/TopIPHC_2012_01_25/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/PU3DMC_Fall11.root";
       //cout << "loading  LumiWeights" <<  endl;
       
       LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       //cout << "PUWeightFileName.c_str()  " << PUWeightFileName.c_str() << endl;
       //cout << " mcfile                   " << mcfile                   << endl;
       //LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       //cout << "LumiWeights loaded" <<  endl;
       
       //if(!IReweight_puUp && !IReweight_puDown)  LumiWeights->weight_init( 1. );
       //if( IReweight_puDown                   )  LumiWeights->weight_init( 1. );
       //if( IReweight_puUp                     )  LumiWeights->weight_init( 1. );
       
     }
     else{
       string datafile = "";
       if(!IReweight_puUp && !IReweight_puDown){
         datafile = "/opt/sbg/data/data1/cms/jandrea/TopIPHC_2012_01_25/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/default73.5mb.root";
       }
       
       if( IReweight_puUp)   datafile = "/opt/sbg/data/data1/cms/jandrea/TopIPHC_2012_01_25/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/default73.5mbUp.root";
       if( IReweight_puDown) datafile = "/opt/sbg/data/data1/cms/jandrea/TopIPHC_2012_01_25/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/default73.5mbDown.root";
       
       string mcfile   = "/opt/sbg/data/data1/cms/jandrea/TopIPHC_2012_01_25/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/PU3DMC.root";
       //cout << "loading  LumiWeights" <<  endl;
       
       LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       //cout << "PUWeightFileName.c_str()  " << PUWeightFileName.c_str() << endl;
       //cout << " mcfile                   " << mcfile                   << endl;
       //LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       //cout << "LumiWeights loaded" <<  endl;
       
       
       
       //LumiWeights3D  = new reweight::Lumi3DReWeighting(mcfile, datafile, "pileup_TTbarSig", "pileup" );
       if(!IReweight_puUp && !IReweight_puDown)  LumiWeights->weight3D_init( 1. );
       if( IReweight_puDown                   )  LumiWeights->weight3D_init( 1. );
       if( IReweight_puUp                     )  LumiWeights->weight3D_init( 1. );
       
     }
   }
   
   //************************************
   
   //cout << "618 " <<  endl;
   //--------------------------------------//
   //   Output file 	
   //--------------------------------------//
   //retrieve info from the input:
   TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");
   //this file will be THE file which will contains all the histograms
   fProofFile = new TProofOutputFile(out->GetTitle());
   // Open the file
   TDirectory *savedir = gDirectory;
   fFile = fProofFile->OpenFile("UPDATE");
   if (fFile && fFile->IsZombie()) SafeDelete(fFile);
   savedir->cd();
   
   
   //this file is very important !!!
   fFile->Write();
   //It is required to add in fOutput the histos you want to feedback
   fOutput->Add(fHist);
   fOutput->Add(fFile);
   cout << "end SlaveBegin " << endl;
}

//_____________________________________________________________________________
Bool_t ProofSelectorMyCutFlow::Process(Long64_t entry)
{
  //---------------------------------------------------//
  // Main event loop: get entry
  //---------------------------------------------------//
  fChain->GetTree()->GetEntry(entry); 
  branch->GetEntry(entry);
  
  //---------------------------------------------------//
  //         Doing the analysis event by event
  //---------------------------------------------------//
  int debugcc=1000;
  int maxdebugcc=10;
  //cout<<"Entry "<<entry<<endl;
  sel.LoadEvent(event);
  
  //Collection of selected objects
  vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
  vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
  vector<NTMuon>     selMuons     = sel.GetSelectedMuons();
  vector<NTJet>      selJets	   = sel.GetSelectedJets();
  NTMET met			   = sel.GetMET(); 
  //NTMET met;
  //met = sel.GetScaledType1METWithJER( selJets,  false,  1,  false,  1); 
  //met = sel.GetType1MET(              selJets,  false,  1,  false,  1); 

  ITypeMC     = -1;
  ICut	  = -1;  
 

  //Candidate pair of lepton
  string CandType; // ee - emu - mumum or false
  vector<TopTree::NTElectron> candElec;
  vector<TopTree::NTMuon> candMuon;
  
  double Dweight[101];
  for(int k1=0; k1<101; k1++) {
    Dweight[k1] = 0.;
  }   
  
  //Manage DY samples to avoid overlaps
  double dileptInvMass = 0;
  if( (event->zAndDecays).size() > 0){
    TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
    dileptInvMass = dilept.M();
  }
  
  //      std::cout<<"dileptInvMass "<<dileptInvMass<<std::endl;
  
  //      if(datasetName=="Zjets" && dileptInvMass < 50 ) cout << "problem !!!" << endl;
  
  //if(datasetName=="Zjets"         && dileptInvMass < 50) return kTRUE;
  //if(datasetName=="DYToLL_M10-50" && dileptInvMass > 50) return kTRUE;
  
  if(datasetName=="DYToMuMu_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) )  return kTRUE;
  if(datasetName=="DYToEE_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) )          return kTRUE;
  if(datasetName=="DYToTauTau_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) )  return kTRUE;
  if(datasetName=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20)      return kTRUE;
  if(datasetName=="DYToEE_M-10To20"	   &&  dileptInvMass > 20)   return kTRUE;
  if(datasetName=="DYToTauTau_M-10To20" &&  dileptInvMass > 20)      return kTRUE;
  
  
  //double weightITypeMC_save = Luminosity*dataset->Xsection()/dataset->getNSkimmedEvent();
  double weightITypeMC_save = Luminosity*dataset->Xsection()/initMCevents[datasetName];
  double weightITypeMC=0;
  
  
  
  //cout << "weightITypeMC_save " << weightITypeMC_save << endl;
  // on introduit ici la loop sur les channels!
  for (int IChannel=0; IChannel<3; IChannel++) {
    //      for (int IChannel=1; IChannel<2; IChannel++) {
    string ChannelName = "";
    if (IChannel==0) ChannelName= "mumu"; 
    else if (IChannel==1) ChannelName= "ee"; 
    else if (IChannel==2) ChannelName= "emu"; 
    
    //if(IChannel==0 ) cout <<  " test mumu 627" << endl;
    if (IChannel==0 && (datasetName=="DataEG" || datasetName=="DataMuEG")) continue;
    if (IChannel==1 && (datasetName=="DataMu" || datasetName=="DataMuEG")) continue;
    if (IChannel==2 && (datasetName=="DataMu" || datasetName=="DataEG"  )) continue;
    
    //if(IChannel==0 ) cout <<  " ok continue " << endl;
    if ( datasetName!="DataEG" && datasetName!="DataMu" && 
	 datasetName!="DataMuEG" && datasetName!="DataEGMu" 
	 && datasetName!="MET1" && datasetName!="MET2") {
      //cout << "IReweight " << IReweight << endl;
      //cout << "IReweight_Spring11" << IReweight_Spring11  << endl;
      
      
      
      
      if(IReweight ){
	
	//old reweighting
	//int npu = event->num_pileup_bc0;
	//if(!IReweight_puUp && !IReweight_puDown) weightITypeMC = weightITypeMC_save*LumiWeights->ITweight(npu);
	//if(IReweight_puUp   ) weightITypeMC = weightITypeMC_save*LumiWeights->ITweight(event->num_pileup_bc0)*PShiftUp_.ShiftWeight( npu );
	//if(IReweight_puDown ) weightITypeMC = weightITypeMC_save*LumiWeights->ITweight(event->num_pileup_bc0)*PShiftDown_.ShiftWeight( npu );
	
	if(datasetName != "DYToLL_M10-50"){
	  weightITypeMC = weightITypeMC_save*LumiWeights->weight3D(event->num_pileup_bcm1 ,event->num_pileup_bc0,event->num_pileup_bcp1);
	}               
	else {
	  double ave_npu = (event->num_pileup_bcm1+event->num_pileup_bc0+event->num_pileup_bcp1)/3.;
	  weightITypeMC = weightITypeMC_save*LumiWeights->ITweight3BX(ave_npu);
	  /*cout << "LumiWeights->ITweight3BX(ave_npu) " << LumiWeights->ITweight3BX(ave_npu) << endl;
	    cout << "ave_npu " <<  ave_npu << endl;
	    cout << "event->num_pileup_bcm1 " <<  event->num_pileup_bcm1 << endl;
	    cout << "event->num_pileup_bc0  " <<  event->num_pileup_bc0  << endl;
	    cout << "event->num_pileup_bcp1 " <<  event->num_pileup_bcp1 << endl;
	    //weightITypeMC = weightITypeMC_save;*/
	}
	
	
      }
      else weightITypeMC = weightITypeMC_save;
      /*
	if (IChannel==2) weightITypeMC = weightITypeMC_save*SF_VxP1[selVertices.size()];   
	else if (IChannel==0) weightITypeMC = weightITypeMC_save*SF_VxP2[selVertices.size()];
	else if (IChannel==1) weightITypeMC = weightITypeMC_save*SF_VxP3[selVertices.size()];
      */
      //         weightITypeMC = weightITypeMC_save*SF_VxP1[selVertices.size()];
      
      
      //cout << "weightITypeMC_save 660 " << weightITypeMC_save << endl;
      //cout << "weightITypeMC      661 " << weightITypeMC << endl;
      //Now replaced by:
      //           weightITypeMC = weightITypeMC_save*sel.GetPUWeight();
    }
    else { // data   
      weightITypeMC = 1;
    }
    
    bool IsSignal = false;
    //int LastStep = 0;
    bool IsData = false;
    double WeightForBranchingRatio = 1.;
    bool IsLJ = false;
    
    
    if ( datasetName=="TTbar" || 
         datasetName=="TTbarScaleUp" ||
         datasetName=="TTbarScaleDown" ||
         datasetName=="TTbarMatchUp" ||
         datasetName=="TTbarMatchDown" ||
         datasetName=="TTbarMass161" ||
         datasetName=="TTbarMass163" ||
         datasetName=="TTbarMass166" ||
         datasetName=="TTbarMass169" ||
         datasetName=="TTbarMass175" ||
         datasetName=="TTbarMass178" ||
         datasetName=="TTbarMass181" ||
         datasetName=="TTbarMass184" ||
         datasetName=="TTbarPUup" ||
         datasetName=="TTbarPUdown" ||
         datasetName=="TTbarPDFup" ||
         datasetName=="TTbarPDFdown" ||
         datasetName=="TTbarLeptUp" ||
         datasetName=="TTbarLeptDown" ||
         datasetName=="TTbarTriggerUp" ||
         datasetName=="TTbarTriggerDown" ||
         datasetName=="TTbarMETUp" ||
         datasetName=="TTbarMETDown" 
	 ) {
      
      if ( IChannel==0) { // "mumu" 
	if ( event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000 )    IsSignal = true;
	if ( !(event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000) ) IsSignal = false;
      }      
      else if ( IChannel==1) {  // "ee" 
	if ( event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200 )     IsSignal = true;
	if ( !(event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200) )  IsSignal = false;
      }      
      else if ( IChannel==2) { // "emu" 
	if ( event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110 )     IsSignal = true;
	if ( !(event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110) )  IsSignal = false;
      }      
      if ( !IsSignal && event->TMEME!=0 )     IsLJ = true;
      if ( IsSignal ) {
        WeightForBranchingRatio = SF_BranchingRatio_ll;
      } else {
        if ( event->TMEME==0 ){
	  WeightForBranchingRatio = SF_BranchingRatio_had;
	} else{
	  WeightForBranchingRatio = SF_BranchingRatio_lj;
	} 
      } 
      
    }
    
    
    vector<NTMuon>     allMuons     = sel.GetMuons();
    
    for(unsigned int imuon=0; imuon<allMuons.size(); imuon++){
      
      double isoRel = allMuons[imuon].RelIso03PF();
      MyhistoManager.FillHisto(LeptonIso_e, "LeptonIso_e", isoRel, datasetName, IsSignal, Dweight[ITypeMC]);
      
    }
    
    vector<NTElectron>     allElectrons     = sel.GetElectrons();
    
    for(unsigned int ielec=0; ielec<allElectrons.size(); ielec++){
      
      double isoRel = allElectrons[ielec].RelIso03PF();
      MyhistoManager.FillHisto(LeptonIso_mu, "LeptonIso_mu", isoRel, datasetName, IsSignal, Dweight[ITypeMC]);
      
    }
    
    
    
    //******************* error cacl ************************
    //Definition of event weight error
    double EventYieldWeightError = 1;
    
    bool isttbar = false;
    
    //cout << "EventYieldWeightError 955 " << EventYieldWeightError << endl;
    
    if ( datasetName=="TTbar" || 
         datasetName=="TTbarScaleUp" ||
         datasetName=="TTbarScaleDown" ||
         datasetName=="TTbarMatchUp" ||
         datasetName=="TTbarMatchDown" ||
         datasetName=="TTbarMass161" ||
         datasetName=="TTbarMass163" ||
         datasetName=="TTbarMass166" ||
         datasetName=="TTbarMass169" ||
         datasetName=="TTbarMass175" ||
         datasetName=="TTbarMass178" ||
         datasetName=="TTbarMass181" ||
         datasetName=="TTbarMass184" ||
         datasetName=="TTbarPUup" ||
         datasetName=="TTbarPUdown" ||
         datasetName=="TTbarPDFup" ||
         datasetName=="TTbarPDFdown" ||
         datasetName=="TTbarLeptUp" ||
         datasetName=="TTbarLeptDown" ||
         datasetName=="TTbarTriggerUp" ||
         datasetName=="TTbarTriggerDown" ||
         datasetName=="TTbarMETUp" ||
         datasetName=="TTbarMETDown" 
    
    
	 ) { 
      isttbar = true;
      datasetName="TTbar";
      if (IsSignal ) { 
	ITypeMC = 0;  
	Dweight[ITypeMC]= weightITypeMC * WeightForBranchingRatio;
	EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
 	if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
 	if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
 	if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
 	if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
 	if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
 	if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      } 
      else     { 
	
	//cout << " Dweight[ITypeMC] 689 " <<weightITypeMC_save << "  ITypeMC " << ITypeMC << endl;
	ITypeMC = 1; 
	Dweight[ITypeMC]= weightITypeMC * WeightForBranchingRatio;
	EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
	if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
	if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
	if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
 	if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
 	if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
 	if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
	fHist->Fill(0., Dweight[ITypeMC] );
	
	//TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
	//TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
	//cout << " weightITypeMC " << weightITypeMC << " Dweight[ITypeMC] 693 " << Dweight[ITypeMC]<< "  ITypeMC " << ITypeMC << endl;
      }
    }
    else if ( datasetName=="Zjets"              || 
	      datasetName=="DYToMuMu_M-20"      || 
	      datasetName=="DYToEE_M-20"        || 
	      datasetName=="DYToTauTau_M-20"    || 
	      datasetName=="DYToMuMu_M-10To20"  || 
	      datasetName=="DYToEE_M-10To20"    || 
	      datasetName=="DYToTauTau_M-10To20" || datasetName=="DYToLL_M10-50"
	      ) { 
      ITypeMC = 2; IsSignal = false; Dweight[ITypeMC]= weightITypeMC;  
      
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
      
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="WjetsMu" ||
	      datasetName=="WjetsE"  ||
	      datasetName=="WjetsTau" ||
	      datasetName=="Wjets"
	      ) { 
      ITypeMC = 3; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
     
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="SingleToptW" || datasetName=="TtW" || datasetName=="TbartW"
	      || datasetName=="TtWScaleUp" || datasetName=="TtWScaleDown"
	      || datasetName=="TbartWScaleUp" || datasetName=="TbartWScaleDown"
	      ) { 
      ITypeMC = 4; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
      
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="WZ" || datasetName=="WW" || datasetName=="ZZ"  || datasetName=="VV") { 
      ITypeMC = 5; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
      fHist->Fill(0.,Dweight[ITypeMC] );
    }
    else if ( datasetName=="DYee" ) { 
      ITypeMC = 6; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC;
      
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
      
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="DYmumu" ) { 
      ITypeMC = 7; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="QCD1" ||
	      datasetName=="QCD2" ||
	      datasetName=="QCD3" ||
	      datasetName=="QCD4" ||
	      datasetName=="QCD5" ||
	      datasetName=="QCD6" ||
	      datasetName=="QCD7" ||
	      datasetName=="QCD8" ||
	      datasetName=="QCD9" ||
	      datasetName=="QCD10" ||
	      datasetName=="QCD11" ||
	      datasetName=="QCD12" ||
	      datasetName=="QCD13" 
	      
	      ) { 
      ITypeMC = 8; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC;
      //******************* error cacl ************************
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
      
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      fHist->Fill(0.,Dweight[ITypeMC] );
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    
    if ( datasetName=="DataEG" || datasetName=="DataMu" || 
	 datasetName=="DataMuEG" || datasetName=="DataEGMu" ||
	 datasetName=="MET1" || datasetName=="MET2") { 
      ITypeMC = 100; IsData = true;  Dweight[ITypeMC]= weightITypeMC; 
      EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];   
      
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 0, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 0, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 0, datasetName, IsSignal, EventYieldWeightError);
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      
    }
    
    string CandType0 = "";
    double LeptonSF      = 0.; 
    double LeptonSFError = 0.; 
    sel.GetLeptonPair(candMuon, candElec, CandType0 ); 
    
    
    if(ApplyLeptonSF){
      if (CandType0=="mumu" &&  !IsData && (  datasetName != "Wjets" &&  datasetName != "QCD"  
					      && !(datasetName  =="TTbar" &&  !IsSignal) )   ) {
	//	  	|| (datasetName  =="TTbar" &&  IsSignal)  )   ) {
	LeptonSF      =sel.getLeptonScaleFactor(     candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), candMuon[1].p4.Pt(), candMuon[1].p4.Eta(), "mumu");
	LeptonSFError =sel.getLeptonScaleFactorError(candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), candMuon[1].p4.Pt(), candMuon[1].p4.Eta(), "mumu");
      }
      if (CandType0=="emu"  &&  !IsData 
	  && ( datasetName != "Wjets" &&  datasetName != "QCD"  
	       && !(datasetName  =="TTbar" &&  !IsSignal) )   ) {
	//		|| (datasetName  =="TTbar" &&  IsSignal)     ) ){
	LeptonSF      =sel.getLeptonScaleFactor(     candElec[0].p4.Pt(), candElec[0].p4.Eta(), candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), "emu");
	LeptonSFError =sel.getLeptonScaleFactorError(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), "emu");
      }
      if (CandType0=="ee"   &&  !IsData 
	  &&  (datasetName != "Wjets" &&  datasetName != "QCD"  
	       && !(datasetName  =="TTbar" &&  !IsSignal) )   ) {
	//		|| (datasetName  =="TTbar" &&  IsSignal)     ) ) {
	LeptonSF     =sel.getLeptonScaleFactor(     candElec[0].p4.Pt(), candElec[0].p4.Eta(), candElec[1].p4.Pt(), candElec[1].p4.Eta(), "ee");
	LeptonSFError=sel.getLeptonScaleFactorError(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candElec[1].p4.Pt(), candElec[1].p4.Eta(), "ee");
      }
    }
    if(producePLRPlots) plotsFurnisher.LoadEvent (sel, ChannelName, IsSignal, ITypeMC, event->TMEME, Dweight[ITypeMC], 
						  *dataset, SF_trig_mu*SF_met_mumu, SF_trig_emu*SF_met_emu, SF_trig_ee*SF_met_ee, SF_e,SF_mu, SF_DY_ee, SF_DY_mm,
						  SF_DY_em,int(selElectrons.size()),CandType0,LeptonSF);
    
    
    //cout << "    CandType0 836" << CandType0 << endl;
    //     Passage trigger
    // Voir DileptonSelection.cc pour conditions de trigger
    //cout << " Dweight[ITypeMC] " << Dweight[ITypeMC] << endl;
    //MyhistoManager.FillHisto(pileup, "pileup" , event->num_pileup_bc0,   datasetName, IsSignal, Dweight[ITypeMC]);
    MyhistoManager.FillHisto(pileup, "pileup" , event->num_pileup_bc0,   datasetName, IsSignal, 1);
    //cout << "test trigger " << ChannelName <<   " " << IsSignal << endl;
    //if(IsSignal) cout << "passsssssssss trigger " << sel.passTriggerSelection ( dataset, ChannelName)<< endl;
    
    //***********************************
    //triger selection
    //***********************************
    if (  sel.passTriggerSelection ( dataset, ChannelName)     ) {
      //if (  ( IsData || sel.passMETTriggerSelection ( dataset).first || sel.passMETTriggerSelection ( dataset).second  )
      //&& sel.passTriggerSelection ( dataset, ChannelName)    ) {
      //         if (  sel.passTriggerSelection ( &datasets[d], ChannelName)     || !IsData ) {
      //cout << "pass trigger " << ChannelName << endl;
      
      
      
      
      if (debugcc<maxdebugcc) {
	cout <<  " seltrigger " ;
      }
      
      
      if (ChannelName=="mumu") MyhistoManager.FillHisto(HNVxP_mumu_aftertrigger, "NVxP_mumu_aftertrigger" ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
      if (ChannelName=="ee")   MyhistoManager.FillHisto(HNVxP_ee_aftertrigger,   "NVxP_ee_aftertrigger"   ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
      if (ChannelName=="emu")  MyhistoManager.FillHisto(HNVxP_emu_aftertrigger,  "NVxP_emu_aftertrigger"  ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 10, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 10, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 10, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 10, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 10, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 10, datasetName, IsSignal, EventYieldWeightError);
      
      
      if(IChannel == 0 && ChannelName=="mumu" &&  !IsData ) { 
	SFtrigger->Fill(0.,SF_trig_mu );
	SFtrigger->Fill(10., 1.);
      }	 
      if(IChannel == 1 && ChannelName=="ee"   &&  !IsData ) { 
	SFtrigger->Fill(1.,SF_trig_ee );
	SFtrigger->Fill(11., 1.);
      }	 
      if(IChannel == 2 && ChannelName=="emu"  &&  !IsData ) { 
	SFtrigger->Fill(2.,SF_trig_emu );
	SFtrigger->Fill(12., 1.);
      } 	 
      
      //TabFlow1[IChannel][ITypeMC][10]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][10]+=Dweight[ITypeMC]*Dweight[ITypeMC];
      
      
      
      
      //         add SF_trigger here   
      if (ChannelName=="mumu" &&  !IsData ){
	Dweight[ITypeMC]*=SF_trig_mu;	
        //******************* error cacl ************************  
	EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC] ;      
      }
      if (ChannelName=="emu"  &&  !IsData ){
	Dweight[ITypeMC]*=SF_trig_emu;	  
        //******************* error cacl ************************  
	EventYieldWeightError =  Dweight[ITypeMC]*Dweight[ITypeMC]  ;        
      }
      if (ChannelName=="ee"   &&  !IsData ){
	Dweight[ITypeMC]*=SF_trig_ee;	
        //******************* error cacl ************************    
	EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC] ;
      }
      
      
      
      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 1, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 1, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 1, datasetName, IsSignal, Dweight[ITypeMC]);
      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 1, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 1, datasetName, IsSignal, EventYieldWeightError);
      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 1, datasetName, IsSignal, EventYieldWeightError);
      
      
      //cout << "EventYieldWeightError 1280 " << EventYieldWeightError << endl;
      
      //sel.GetLeptonPair(candMuon, candElec, CandType ); 
      
      
      
      if(datasetName=="DataEG" || datasetName=="DataMu"  
	 ||  datasetName=="DataEGMu" ||  datasetName=="DataMuEG"){
	//sel.GetLeptonPairElectronScaled(candMuon, candElec, CandType, 1. ); 
	scaleElec = 1.00;
	sel.GetLeptonPairElectronScaled(candMuon, candElec, CandType, scaleElec ); 
      }
      
      if(datasetName!="DataEG" && datasetName!="DataMu"  
	 &&  datasetName!="DataMuEG" &&  datasetName!="DataEGMu"){
	//cout << "tmeme " << event->TMEME << endl;
	//sel.GetLeptonPairElectronSmeared(candMuon, candElec, CandType, 1, 0 ); 
	resolElec= 0.00;
	sel.GetLeptonPairElectronSmeared(candMuon, candElec, CandType, resolElec ); 
      }
      
      
      
      
      //*******************
      //fill histograms met
      
      TLorentzVector DiLepton_mumu;
      for(unsigned int i=0;i<candMuon.size();i++) DiLepton_mumu+=candMuon[i].p4;
      TLorentzVector DiLepton_ee;
      for(unsigned int i=0;i<candElec.size();i++) DiLepton_ee+=candElec[i].p4;
      TLorentzVector DiLepton_emu;
      for(unsigned int i=0;i<candMuon.size();i++) DiLepton_emu+=candMuon[i].p4;
      for(unsigned int i=0;i<candElec.size();i++) DiLepton_emu+=candElec[i].p4;
      
      float InvDilMass = 0;
      if (ChannelName=="mumu")  InvDilMass = DiLepton_mumu.M();
      if (ChannelName=="ee")    InvDilMass = DiLepton_ee.M();
      if (ChannelName=="emu")   InvDilMass = DiLepton_emu.M();
      
      
      // compute DeltaPhi
      float deltaPhi_met1 = 1000;
      float deltaPhi_met2 = 1000;
      float deltaPhi_met  = 1000;
      float deltaDileptonMET = 1000;
      
      double closestDeltaPhiJetMet = 1000;
      double deltaPhi_ClosestDiLeptonJets= 1000;
      
      if(met.p4.Pt() > 30){
	
	//cout << "    CandType 941" << CandType << endl;
        if (ChannelName=="mumu" && CandType=="mumu")   {
	  deltaPhi_met1 = fabs(candMuon[0].p4.DeltaPhi(met.p4));
	  deltaPhi_met2 = fabs(candMuon[1].p4.DeltaPhi(met.p4));
	  deltaDileptonMET = fabs(DiLepton_mumu.DeltaPhi(met.p4)); 
          MyhistoManager.FillHisto(HDeltaPhi_mumu_LeptonMET,      "HDeltaPhi_mumu_LeptonMET"      , deltaPhi_met1, datasetName, IsSignal, Dweight[ITypeMC]);
          MyhistoManager.FillHisto(HDeltaPhi_mumu_LeptonMET,      "HDeltaPhi_mumu_LeptonMET"      , deltaPhi_met2, datasetName, IsSignal, Dweight[ITypeMC]);
          MyhistoManager.FillHisto(HDeltaPhi_mumu_DiLeptonMET,    "HDeltaPhi_mumu_DiLeptonMET"    , deltaDileptonMET, datasetName, IsSignal, Dweight[ITypeMC]); 	   
        }
        else if (ChannelName=="ee" && CandType=="ee") {
	  deltaPhi_met1 = fabs(candElec[0].p4.DeltaPhi(met.p4));
	  deltaPhi_met2 = fabs(candElec[1].p4.DeltaPhi(met.p4));
	  deltaDileptonMET = fabs(DiLepton_ee.DeltaPhi(met.p4));
          MyhistoManager.FillHisto(HDeltaPhi_ee_LeptonMET,      "HDeltaPhi_ee_LeptonMET"      , deltaPhi_met1, datasetName, IsSignal, Dweight[ITypeMC]);
          MyhistoManager.FillHisto(HDeltaPhi_ee_LeptonMET,      "HDeltaPhi_ee_LeptonMET"      , deltaPhi_met2, datasetName, IsSignal, Dweight[ITypeMC]);
          MyhistoManager.FillHisto(HDeltaPhi_ee_DiLeptonMET,    "HDeltaPhi_ee_DiLeptonMET"    , deltaDileptonMET, datasetName, IsSignal, Dweight[ITypeMC]); 	   
        }
	
	
        else if (ChannelName=="emu" && CandType=="emu") {
	  deltaPhi_met1 = fabs(candElec[0].p4.DeltaPhi(met.p4));
	  deltaPhi_met2 = fabs(candMuon[0].p4.DeltaPhi(met.p4));
	  deltaDileptonMET = fabs(DiLepton_emu.DeltaPhi(met.p4));
          MyhistoManager.FillHisto(HDeltaPhi_emu_LeptonMET,      "HDeltaPhi_emu_LeptonMET"      , deltaPhi_met1, datasetName, IsSignal, Dweight[ITypeMC]);
          MyhistoManager.FillHisto(HDeltaPhi_emu_LeptonMET,      "HDeltaPhi_emu_LeptonMET"      , deltaPhi_met2, datasetName, IsSignal, Dweight[ITypeMC]);
          MyhistoManager.FillHisto(HDeltaPhi_emu_DiLeptonMET,    "HDeltaPhi_emu_DiLeptonMET"    , deltaDileptonMET, datasetName, IsSignal, Dweight[ITypeMC]); 	   
	}
	
      }
      
      
      
      
      for(unsigned int ijet = 0; ijet < selJets.size(); ijet++){
        TLorentzVector P4jets = selJets[ijet].p4;
	
	double deltaPhi_JetMet = fabs(P4jets.DeltaPhi(met.p4));   
	if(deltaPhi_JetMet< closestDeltaPhiJetMet) closestDeltaPhiJetMet = deltaPhi_JetMet;
	
	
	
	
	double deltaPhi_DiLeptonJets = -1; 
	
	if(CandType=="ee"   && ChannelName=="ee" && met.p4.Pt() > 30 ){
	  deltaPhi_DiLeptonJets= fabs(P4jets.DeltaPhi(DiLepton_ee));   
	  
	  if(deltaPhi_DiLeptonJets < deltaPhi_ClosestDiLeptonJets ) deltaPhi_ClosestDiLeptonJets = deltaPhi_DiLeptonJets;
	  
	  MyhistoManager.FillHisto(HDeltaPhi_ee_DiLeptonJets,   "HDeltaPhi_ee_DiLeptonJets"   , deltaPhi_DiLeptonJets , datasetName, IsSignal, Dweight[ITypeMC]); 	   
          MyhistoManager.FillHisto(HDeltaPhi_ee_JetsMET,        "HDeltaPhi_ee_JetsMET"        , deltaPhi_JetMet       , datasetName, IsSignal, Dweight[ITypeMC]);   
          if(DiLepton_ee.M() > 76 && DiLepton_ee.M() < 106) MyhistoManager.FillHisto(HDeltaPhi_ee_PtJetsInMassP,  "HDeltaPhi_ee_PtJetsInMassP"  , selJets[ijet].p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]);   
          if(DiLepton_ee.M() < 76 || DiLepton_ee.M() > 106) MyhistoManager.FillHisto(HDeltaPhi_ee_PtJetsOutMassP, "HDeltaPhi_ee_PtJetsOutMassP" , selJets[ijet].p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 
        }
	
	if(CandType=="emu"   && ChannelName=="emu" && met.p4.Pt() > 30){
	  deltaPhi_DiLeptonJets= fabs(P4jets.DeltaPhi(DiLepton_emu));   
	  if(deltaPhi_DiLeptonJets < deltaPhi_ClosestDiLeptonJets ) deltaPhi_ClosestDiLeptonJets = deltaPhi_DiLeptonJets;
	  MyhistoManager.FillHisto(HDeltaPhi_emu_DiLeptonJets,   "HDeltaPhi_emu_DiLeptonJets"   , deltaPhi_DiLeptonJets , datasetName, IsSignal, Dweight[ITypeMC]); 	   
          MyhistoManager.FillHisto(HDeltaPhi_emu_JetsMET,        "HDeltaPhi_emu_JetsMET"        , deltaPhi_JetMet       , datasetName, IsSignal, Dweight[ITypeMC]);   
          if(DiLepton_emu.M() > 76 && DiLepton_emu.M() < 106) MyhistoManager.FillHisto(HDeltaPhi_emu_PtJetsInMassP,  "HDeltaPhi_emu_PtJetsInMassP"  , selJets[ijet].p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]);   
          if(DiLepton_emu.M() < 76 || DiLepton_emu.M() > 106) MyhistoManager.FillHisto(HDeltaPhi_emu_PtJetsOutMassP, "HDeltaPhi_emu_PtJetsOutMassP" , selJets[ijet].p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 
        }
        
	if(CandType=="mumu"   && ChannelName=="mumu" && met.p4.Pt() > 30){
	  deltaPhi_DiLeptonJets= fabs(P4jets.DeltaPhi(DiLepton_mumu)); 
	  if(deltaPhi_DiLeptonJets < deltaPhi_ClosestDiLeptonJets ) deltaPhi_ClosestDiLeptonJets = deltaPhi_DiLeptonJets;  
	  MyhistoManager.FillHisto(HDeltaPhi_mumu_DiLeptonJets,   "HDeltaPhi_mumu_DiLeptonJets"   , deltaPhi_DiLeptonJets , datasetName, IsSignal, Dweight[ITypeMC]); 	   
          MyhistoManager.FillHisto(HDeltaPhi_mumu_JetsMET,        "HDeltaPhi_mumu_JetsMET"        , deltaPhi_JetMet       , datasetName, IsSignal, Dweight[ITypeMC]);   
          
	  if(DiLepton_mumu.M() > 76 && DiLepton_mumu.M() < 106) MyhistoManager.FillHisto(HDeltaPhi_mumu_PtJetsInMassP,  "HDeltaPhi_mumu_PtJetsInMassP"  , selJets[ijet].p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]);   
          if(DiLepton_mumu.M() < 76 || DiLepton_mumu.M() > 106) MyhistoManager.FillHisto(HDeltaPhi_mumu_PtJetsOutMassP, "HDeltaPhi_mumu_PtJetsOutMassP" , selJets[ijet].p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 
        }
      }
      
      if(met.p4.Pt() > 30){
	
	//cout << "    CandType 941" << CandType << endl;
	if (ChannelName=="mumu" && CandType=="mumu")   {
	  deltaPhi_met1 = fabs(candMuon[0].p4.DeltaPhi(met.p4));
	  deltaPhi_met2 = fabs(candMuon[1].p4.DeltaPhi(met.p4));          
	  MyhistoManager.FillHisto(HDeltaPhi_mumu_ClosestJetsMET, "HDeltaPhi_mumu_ClosestJetsMET"  ,  closestDeltaPhiJetMet   , datasetName, IsSignal, Dweight[ITypeMC]);   
	  MyhistoManager.FillHisto(HDeltaPhi_mumu_ClosestDiLeptonJets,   "HDeltaPhi_mumu_ClosestDiLeptonJets"   , deltaPhi_ClosestDiLeptonJets , datasetName, IsSignal, Dweight[ITypeMC]); 	   
	  deltaDileptonMET = fabs(DiLepton_mumu.DeltaPhi(met.p4)); 
	  MyhistoManager.FillHisto(HDeltaPhi_mumu_LeptonMET,      "HDeltaPhi_mumu_LeptonMET"      , deltaPhi_met1, datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhi_mumu_LeptonMET,      "HDeltaPhi_mumu_LeptonMET"      , deltaPhi_met2, datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhi_mumu_DiLeptonMET,    "HDeltaPhi_mumu_DiLeptonMET"    , deltaDileptonMET, datasetName, IsSignal, Dweight[ITypeMC]); 	
	  MyhistoManager.FillHisto(HDileptPt_mumu,    "HDileptPt_mumu"    , DiLepton_mumu.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 	
	}
	else if (ChannelName=="ee" && CandType=="ee" ) {
	  deltaPhi_met1 = fabs(candElec[0].p4.DeltaPhi(met.p4));
	  deltaPhi_met2 = fabs(candElec[1].p4.DeltaPhi(met.p4));
	  deltaDileptonMET = fabs(DiLepton_ee.DeltaPhi(met.p4));	
	  MyhistoManager.FillHisto(HDeltaPhi_ee_ClosestJetsMET, "HDeltaPhi_ee_ClosestJetsMET"  ,  closestDeltaPhiJetMet   , datasetName, IsSignal, Dweight[ITypeMC]);   
	  MyhistoManager.FillHisto(HDeltaPhi_ee_ClosestDiLeptonJets,   "HDeltaPhi_ee_ClosestDiLeptonJets"   , deltaPhi_ClosestDiLeptonJets , datasetName, IsSignal, Dweight[ITypeMC]); 	   
	  MyhistoManager.FillHisto(HDeltaPhi_ee_LeptonMET,      "HDeltaPhi_ee_LeptonMET"      , deltaPhi_met1, datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhi_ee_LeptonMET,      "HDeltaPhi_ee_LeptonMET"      , deltaPhi_met2, datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhi_ee_DiLeptonMET,    "HDeltaPhi_ee_DiLeptonMET"    , deltaDileptonMET, datasetName, IsSignal, Dweight[ITypeMC]); 
	  MyhistoManager.FillHisto(HDileptPt_ee,    "HDileptPt_ee"    , DiLepton_ee.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 		   
	}
	
	
	else if (ChannelName=="emu" && CandType=="emu" ) {
	  deltaPhi_met1 = fabs(candElec[0].p4.DeltaPhi(met.p4));
	  deltaPhi_met2 = fabs(candMuon[0].p4.DeltaPhi(met.p4));
	  deltaDileptonMET = fabs(DiLepton_emu.DeltaPhi(met.p4));	
	  MyhistoManager.FillHisto(HDeltaPhi_emu_ClosestJetsMET, "HDeltaPhi_emu_ClosestJetsMET"  ,  closestDeltaPhiJetMet   , datasetName, IsSignal, Dweight[ITypeMC]);   
	  MyhistoManager.FillHisto(HDeltaPhi_emu_ClosestDiLeptonJets,   "HDeltaPhi_emu_ClosestDiLeptonJets"   , deltaPhi_ClosestDiLeptonJets , datasetName, IsSignal, Dweight[ITypeMC]); 	   
	  MyhistoManager.FillHisto(HDeltaPhi_emu_LeptonMET,      "HDeltaPhi_emu_LeptonMET"      , deltaPhi_met1, datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhi_emu_LeptonMET,      "HDeltaPhi_emu_LeptonMET"      , deltaPhi_met2, datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhi_emu_DiLeptonMET,    "HDeltaPhi_emu_DiLeptonMET"    , deltaDileptonMET, datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDileptPt_emu,    "HDileptPt_emu"    , DiLepton_emu.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 	 	   
	}
      }
      //*******************
      
      
      
      if (ChannelName=="mumu"&& CandType=="mumu")  {
        InvDilMass = DiLepton_mumu.M();	
	MyhistoManager.FillHisto2D(HDileptPt_vs_met_mumu, "HDileptPt_vs_met_mumu", DiLepton_mumu.Pt(), met.p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 
      }
      if (ChannelName=="ee" && CandType=="ee")  {
        InvDilMass = DiLepton_ee.M();
	MyhistoManager.FillHisto2D(HDileptPt_vs_met_ee, "HDileptPt_vs_met_ee", DiLepton_ee.Pt(), met.p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 
      }
      if (ChannelName=="emu" && CandType=="emu")  {
	InvDilMass = DiLepton_emu.M();
	MyhistoManager.FillHisto2D(HDileptPt_vs_met_emu, "HDileptPt_vs_met_emu", DiLepton_emu.Pt(), met.p4.Pt(), datasetName, IsSignal, Dweight[ITypeMC]); 
      }
      
      
      
      //********************************************
      // keep track before SF_Id application (Caro)
      //For CutFlow tables, for the nominal values
      //********************************************
      if ( ((CandType=="mumu" && ChannelName=="mumu")   || (CandType=="ee"   && ChannelName=="ee")     || (CandType=="emu"  && ChannelName=="emu")     )
	   && selVertices.size()>0 && InvDilMass> sel.GetMinValueMassCut() ) {
	
	
	
	if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 11, datasetName, IsSignal, Dweight[ITypeMC]);
	if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 11, datasetName, IsSignal, Dweight[ITypeMC]);
	if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 11, datasetName, IsSignal, Dweight[ITypeMC]);
	//cout << "EventYieldWeightError 1504 " << EventYieldWeightError << endl;
	fHist->Fill(2.,Dweight[ITypeMC] );
      }
      
      
      
      double LeptSF_error =0;
      if(ApplyLeptonSF){
	if (ChannelName=="ee" ) {
	  //	   std::cout<<"avant applic SF weight = " << Dweight[ITypeMC]<<std::endl;
	} 
	
	///////////
	if (CandType=="mumu" &&  !IsData && (  datasetName != "Wjets" &&  datasetName != "QCD"  
					       && !(datasetName  =="TTbar" &&  !IsSignal) )   ) {
	  //	  	|| (datasetName  =="TTbar" &&  IsSignal)  )   ) {	  
	  sumSFlept_mumu +=  Dweight[ITypeMC];
	  Dweight[ITypeMC]*=sel.getLeptonScaleFactor(candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), candMuon[1].p4.Pt(), candMuon[1].p4.Eta(), "mumu");
	  //EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC] ;
	  LeptSF_error=sel.getLeptonScaleFactorError(candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), candMuon[1].p4.Pt(), candMuon[1].p4.Eta(), "mumu");
	  
	  nEvents_mumu += Dweight[ITypeMC];
	}
	if (CandType=="emu"  &&  !IsData 
	    && ( datasetName != "Wjets" &&  datasetName != "QCD"  
		 && !(datasetName  =="TTbar" &&  !IsSignal) )   ) {
	  //		|| (datasetName  =="TTbar" &&  IsSignal)     ) ){	  
	  sumSFlept_emu += Dweight[ITypeMC];	
	  Dweight[ITypeMC]     *=sel.getLeptonScaleFactor(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), "emu");	  
	  //EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC];
	  LeptSF_error          = sel.getLeptonScaleFactorError(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), "emu");	  
	  
	  nEvents_emu += Dweight[ITypeMC];
	}
	if (CandType=="ee"   &&  !IsData 
	    &&  (datasetName != "Wjets" &&  datasetName != "QCD"  
		 && !(datasetName  =="TTbar" &&  !IsSignal) )   ) {
	  //		|| (datasetName  =="TTbar" &&  IsSignal)     ) ) {	  
	  sumSFlept_ee += Dweight[ITypeMC];
	  Dweight[ITypeMC]     *= sel.getLeptonScaleFactor(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candElec[1].p4.Pt(), candElec[1].p4.Eta(), "ee");
	  //EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC] ;
	  LeptSF_error          = sel.getLeptonScaleFactorError(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candElec[1].p4.Pt(), candElec[1].p4.Eta(), "ee");
	  nEvents_ee += Dweight[ITypeMC];
	}
	if (ChannelName=="ee" ) {
	  //	   std::cout<<"apres applic SF weight = " << Dweight[ITypeMC]<<std::endl;
	} 
      }
      //cout << "EventYieldWeightError 1540 " << EventYieldWeightError<< endl;
      
      //********************************************
      // keep track before SF_Id application (Caro)
      // For CutFlow tables, for the uncertainties
      //********************************************
      if ( ((CandType=="mumu" && ChannelName=="mumu")   || (CandType=="ee"   && ChannelName=="ee")     || (CandType=="emu"  && ChannelName=="emu")     )
	   && selVertices.size()>0 && InvDilMass> sel.GetMinValueMassCut() ) {
	
	
	
 	if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 11, datasetName, IsSignal, LeptSF_error*LeptSF_error);
 	if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 11, datasetName, IsSignal, LeptSF_error*LeptSF_error);
 	if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 11, datasetName, IsSignal, LeptSF_error*LeptSF_error);
	//cout << "EventYieldWeightError 1504 " << EventYieldWeightError << endl;
	fHist->Fill(2.,Dweight[ITypeMC] );
      }
      
      
      // end of keep track
      
      //      Passage dilepton candidat
      if ( ((CandType=="mumu" && ChannelName=="mumu")   || 
	    (CandType=="ee"   && ChannelName=="ee")     ||
	    (CandType=="emu"  && ChannelName=="emu")     )
	   && selVertices.size()>0 
	   && InvDilMass> sel.GetMinValueMassCut() ) {
	if (debugcc<maxdebugcc) {
	  cout << " selpair " ;
	}
	
	//	  TabFlow1[IChannel][ITypeMC][11]+=Dweight[ITypeMC];     // DONE BEFORE sel.getLeptonScaleFactor !!!
	//	  TabFlow2[IChannel][ITypeMC][11]+=Dweight[ITypeMC]*Dweight[ITypeMC];
	
	
	double initWeight = Dweight[ITypeMC];
	
	      // APPLY SF_DD HERE! :
	      if (ChannelName=="mumu"){ 	  
		
		if (isttbar && !IsSignal  ) { 
		    Dweight[ITypeMC]*=vSF_FakeBck_mm[0]; 
		    EventYieldWeightError = vSF_FakeBck_mm_error[0]*initWeight;
		 }
	      }
	      else if (ChannelName=="emu" ){
		if (isttbar && !IsSignal ) { 
		    Dweight[ITypeMC]*=vSF_FakeBck_em[0]; 
		    EventYieldWeightError = vSF_FakeBck_em_error[0]*initWeight;
		}
	      }
	      else if (ChannelName=="ee"  ){
		if (isttbar && !IsSignal ) { 
		    Dweight[ITypeMC]*=vSF_FakeBck_ee[0]; 
		    EventYieldWeightError = vSF_FakeBck_ee_error[0]*initWeight;
		}
	      }
	
	
 	if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 2, datasetName, IsSignal, Dweight[ITypeMC]);
	if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 2, datasetName, IsSignal, Dweight[ITypeMC]);
	if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 2, datasetName, IsSignal, Dweight[ITypeMC]);
 	if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 2, datasetName, IsSignal, EventYieldWeightError);
 	if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 2, datasetName, IsSignal, EventYieldWeightError);
 	if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 2, datasetName, IsSignal, EventYieldWeightError);
	
	//TabFlow1[IChannel][ITypeMC][2]+=Dweight[ITypeMC];
	//TabFlow2[IChannel][ITypeMC][2]+=Dweight[ITypeMC]*Dweight[ITypeMC];
	
	//DYEstimation
	if (ChannelName=="emu" && sel.GetSelectedJets(candMuon, candElec).size()>=2 ) {
	  
	  if ( !IsData ){
	    if ( ((event->TMEME ==11)||(event->TMEME ==21100)||(event->TMEME ==11001)||(event->TMEME ==10110)) && datasetName=="TTbar" ) {
	      MyhistoManager.FillHisto(hlistemu_ee,"hlistemu_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
	      MyhistoManager.FillHisto(hlistemu_mumu,"hlistemu_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
	    }
	    if ( !(((event->TMEME ==11)||(event->TMEME ==21100)||(event->TMEME ==11001)||(event->TMEME ==10110))) && datasetName=="TTbar" ) {
	      MyhistoManager.FillHisto(hlistemu_ee,"hlistemu_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	      MyhistoManager.FillHisto(hlistemu_mumu,"hlistemu_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	    }
	  } else {
	    if (CandType=="emu" && IsData) MyhistoManager.FillHisto(hemudata_emu,"hemudata_emu",InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);	      
	  }
	  
	}
	//EndDYEstimation
	
	
	vector<NTJet>      theselJets = sel.GetSelectedJets(candMuon, candElec);
	int JetMul = theselJets.size();
	if ( JetMul>3 ) JetMul=4;
	
	float misset = met.p4.Et();
	
    
  
  
	int NBtaggedJets = 0;
	int AlgoBtag = sel.GetbtagAlgo();
	for(unsigned int j=0;j<theselJets.size();j++){
	  //if ( AlgoBtag==0 &&  theselJets[j].TCDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	  //if ( AlgoBtag==1 &&  theselJets[j].SVDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	  //if ( AlgoBtag==2 &&  theselJets[j].SMDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	  
	  if ( AlgoBtag==0 &&  theselJets[j].GetDiscri(string("trackCountingHighEffBJetTags"))	      >=sel.GetbtagDiscriCut ()) NBtaggedJets++;
          if ( AlgoBtag==1 &&  theselJets[j].GetDiscri(string("simpleSecondaryVertexHighEffBJetTags"))>=sel.GetbtagDiscriCut ()) NBtaggedJets++;
          if ( AlgoBtag==2 &&  theselJets[j].GetDiscri(string("trackCountingHighPurBJetTags"))	      >=sel.GetbtagDiscriCut ()) NBtaggedJets++;
          if ( AlgoBtag==3 &&  theselJets[j].GetDiscri(string("simpleSecondaryVertexHighPurBJetTags"))>=sel.GetbtagDiscriCut ()) NBtaggedJets++;
          if ( AlgoBtag==4 &&  theselJets[j].GetDiscri(string("jetProbabilityBJetTags"))  	      >=sel.GetbtagDiscriCut ()) NBtaggedJets++;
          if ( AlgoBtag==5 &&  theselJets[j].GetDiscri(string("jetBProbabilityBJetTags")) 	      >=sel.GetbtagDiscriCut ()) NBtaggedJets++;
          if ( AlgoBtag==6 &&  theselJets[j].GetDiscri(string("combinedSecondaryVertexBJetTags"))     >=sel.GetbtagDiscriCut ()) NBtaggedJets++;
	  
	}
	
	// weightb
	vector < float >weightb;
	weightb.push_back (1.);
	weightb.push_back (0.);
	weightb.push_back (0.);
	weightb.push_back (0.);
	weightb.push_back (0.);
	if (sel.GetFlagb() == 1) {
	  if (!IsData) {      //MC 
	    
	    
	    
	    //vector < float >weight_temp = sel.GetSFBweight().GetWeigth4BSel (sel.GetMethodb(), sel.GetSystb(),theselJets);
	    //vector < float > weight_temp = sel.GetSFBweight().GetWeigth4BSel (sel.GetMethodb(), sel.GetSystb(),theselJets);
	    vector < float > weight_temp =   sel.GetSFBweight ().GetWeigth4BSel (sel.GetMethodb (), sel.GetSystb (), theselJets);
	    weightb[0] = weight_temp[0]; //weight of the event
	    weightb[1] = weight_temp[1];  //proba 0 jet
	    weightb[2] = weight_temp[2];  //proba 1 jet
	    weightb[3] = weight_temp[3];  //proba 2 jets
	    weightb[4] = weight_temp[4];  //proba at least 3 jets
	  }
	  
	}
	
	if( fabs(met.p4.Et() - met.p4.Pt()) > 0000001 ){
	  cout << met.p4.Et()  << "  " <<  met.p4.Pt() << endl;
	
	}
	//cout << "ChannelName test mumu" <<  ChannelName << endl;
	if (ChannelName=="mumu"  &&  InvDilMass> sel.GetMinValueMassCut())  {
	  //cout << "pass mumu check" <<  ChannelName<< endl; 
	         
	  MyhistoManager.FillHisto(HInvM_ll_pair_mumu, "InvDilMassPair_mumu" ,InvDilMass,datasetName, IsSignal, Dweight[ITypeMC]);
	  if( fabs(candMuon[0].p4.Eta()) <1.5 &&  fabs(candMuon[1].p4.Eta()) <1.5 )
	    MyhistoManager.FillHisto(HInvM_ll_pair_mumu_inf15, "InvDilMassPair_mumu_inf15" ,InvDilMass,datasetName, IsSignal, Dweight[ITypeMC]);
	  if( fabs(candMuon[0].p4.Eta()) >1.5 &&  fabs(candMuon[1].p4.Eta()) >1.5 )
	    MyhistoManager.FillHisto(HInvM_ll_pair_mumu_sup15, "InvDilMassPair_mumu_sup15" ,InvDilMass,datasetName, IsSignal, Dweight[ITypeMC]);
	  if ( theselJets.size()<=1 )   MyhistoManager.FillHisto(MyHistos_mumu, "InvDilMassNJinf1_mumu",InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	  else   MyhistoManager.FillHisto(MyHistos1_mumu,"InvDilMassNJsup1_mumu",InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	  
          
	  if(met.p4.Pt() <10) MyhistoManager.FillHisto2D(HInvM_mumu_pair_vs_NBJet_METinf10, "HInvM_mumu_pair_vs_NBJet_METinf10", InvDilMass, NBtaggedJets,datasetName, IsSignal, Dweight[ITypeMC]); 	      
	  if(met.p4.Pt() <10) MyhistoManager.FillHisto2D(HInvM_mumu_pair_vs_NJet_METinf10,  "HInvM_mumu_pair_vs_NJet_METinf10" , InvDilMass, theselJets.size(),datasetName, IsSignal, Dweight[ITypeMC]); 
	  if(theselJets.size()==0) MyhistoManager.FillHisto2D(HInvM_mumu_pair_vs_MET_NJet0, "HInvM_mumu_pair_vs_MET_NJet0"     , InvDilMass, met.p4.Pt(),datasetName, IsSignal, Dweight[ITypeMC]); 
          if(NBtaggedJets     ==0) MyhistoManager.FillHisto2D(HInvM_mumu_pair_vs_MET_NBJet0, "HInvM_mumu_pair_vs_MET_NBJet0"     , InvDilMass, met.p4.Pt(),datasetName, IsSignal, Dweight[ITypeMC]); 	 
	  MyhistoManager.FillHisto3D(HInvM_mumu_pair_vs_MET_NJet, "HInvM_mumu_pair_vs_MET_NJet"     , InvDilMass, met.p4.Pt(), theselJets.size(),datasetName, IsSignal, Dweight[ITypeMC]); 
	  MyhistoManager.FillHisto2D(HInvM_mumu_pair_vs_PU,"HInvM_mumu_pair_vs_PU", InvDilMass, selVertices.size(), datasetName, IsSignal, Dweight[ITypeMC]); 
	   
          MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMDilepton_mumu, "HInvM_ll_afterSelButInvMDilepton_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	  
          if(met.p4.Et() > sel.GetMETCut().second ){
	    MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_mumu, "HInvM_ll_afterSelButInvMNJet_AfterMET_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	    double theweight = Dweight[ITypeMC];
	    if(ITypeMC == 2) theweight = Dweight[ITypeMC]*1.91463;
	    MyhistoManager.FillHisto(HNjets_mumu_aftermetcutbutjetcut, "Njets_mumu_aftermetcutbutjetcut" , JetMul, datasetName, IsSignal, theweight); 
       
	    }
	   
	  if( theselJets.size()>=2 ){		
	    
	    MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMNJet_mumu, "HInvM_ll_afterSelButInvMNJet_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	    
	    if(met.p4.Et() > sel.GetMETCut().second ){
	      
	      if(sel.GetFlagb() == 0 || IsData){
		if( NBtaggedJets >=0 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup0Btag_mumu, "HInvM_ll_afterSelButInvMSup0Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets >=1 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup1Btag_mumu, "HInvM_ll_afterSelButInvMSup1Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets >=2 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup2Btag_mumu, "HInvM_ll_afterSelButInvMSup2Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);		
	      	
		if( NBtaggedJets ==0 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM0Btag_mumu, "HInvM_ll_afterSelButInvM0Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets ==1 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM1Btag_mumu, "HInvM_ll_afterSelButInvM1Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets ==2 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM2Btag_mumu, "HInvM_ll_afterSelButInvM2Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);		
	      }
	      if(sel.GetFlagb() == 1 && !IsData){
		
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup0Btag_mumu, "HInvM_ll_afterSelButInvMSup0Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup1Btag_mumu, "HInvM_ll_afterSelButInvMSup1Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*(1-weightb[1] ) );
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup2Btag_mumu, "HInvM_ll_afterSelButInvMSup2Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[0]);		 
	      	
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM0Btag_mumu, "HInvM_ll_afterSelButInvM0Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[1]);
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM1Btag_mumu, "HInvM_ll_afterSelButInvM1Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[2]);
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM2Btag_mumu, "HInvM_ll_afterSelButInvM2Btag_mumu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[3]);  
		
	      }
	    }
	  }
	}
	else if (ChannelName=="ee"  &&  InvDilMass> sel.GetMinValueMassCut()) {
	  MyhistoManager.FillHisto(HInvM_ll_pair_ee, "InvDilMassPair_ee" ,InvDilMass,datasetName, IsSignal, Dweight[ITypeMC]);
	  if( fabs(candElec[0].p4.Eta()) <1.5 &&  fabs(candElec[1].p4.Eta()) <1.5 )
	    MyhistoManager.FillHisto(HInvM_ll_pair_ee_inf15, "InvDilMassPair_ee_inf15" ,InvDilMass,datasetName, IsSignal, Dweight[ITypeMC]);
	  if( fabs(candElec[0].p4.Eta()) >1.5 &&  fabs(candElec[1].p4.Eta()) >1.5 )
	    MyhistoManager.FillHisto(HInvM_ll_pair_ee_sup15, "InvDilMassPair_ee_sup15" ,InvDilMass,datasetName, IsSignal, Dweight[ITypeMC]);
	  if ( theselJets.size()<=1 )   MyhistoManager.FillHisto(MyHistos_ee, "InvDilMassNJinf1_ee",InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	    else   MyhistoManager.FillHisto(MyHistos1_ee,"InvDilMassNJsup1_ee",InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	  
	  if(met.p4.Pt() <10)       MyhistoManager.FillHisto2D(HInvM_ee_pair_vs_NBJet_METinf10,"HInvM_ee_pair_vs_NBJet_METinf10", InvDilMass,NBtaggedJets ,datasetName, IsSignal, Dweight[ITypeMC]); 
	  if(met.p4.Pt() <10) MyhistoManager.FillHisto2D(HInvM_ee_pair_vs_NJet_METinf10,       "HInvM_ee_pair_vs_NJet_METinf10", InvDilMass, theselJets.size(),datasetName, IsSignal, Dweight[ITypeMC]); 
	  if(theselJets.size()==0) MyhistoManager.FillHisto2D(HInvM_ee_pair_vs_MET_NJet0,      "HInvM_ee_pair_vs_MET_NJet0", InvDilMass,met.p4.Pt(),datasetName, IsSignal, Dweight[ITypeMC]); 
	  if(NBtaggedJets     ==0) MyhistoManager.FillHisto2D(HInvM_ee_pair_vs_MET_NBJet0,     "HInvM_ee_pair_vs_MET_NBJet0", InvDilMass,met.p4.Pt(),datasetName, IsSignal, Dweight[ITypeMC]); 	       
	  MyhistoManager.FillHisto2D(HInvM_ee_pair_vs_PU,"HInvM_ee_pair_vs_PU", InvDilMass, selVertices.size(), datasetName, IsSignal, Dweight[ITypeMC]); 
	  MyhistoManager.FillHisto3D(HInvM_ee_pair_vs_MET_NJet, "HInvM_ee_pair_vs_MET_NJet"     , InvDilMass, met.p4.Pt(), theselJets.size(),datasetName, IsSignal, Dweight[ITypeMC]); 


          MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMDilepton_ee, "HInvM_ll_afterSelButInvMDilepton_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	  
	  if(met.p4.Et() > sel.GetMETCut().second ) {
	    MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_ee, "HInvM_ll_afterSelButInvMNJet_AfterMET_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]); 
	    double theweight = Dweight[ITypeMC];
	    if(ITypeMC == 2) theweight = Dweight[ITypeMC]*2.16651;

            MyhistoManager.FillHisto(HNjets_ee_aftermetcutbutjetcut,   "Njets_ee_aftermetcutbutjetcut"   , JetMul, datasetName, IsSignal, theweight); 
	    }

	  if(  theselJets.size()>=2 ){
	    
	    MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMNJet_ee, "HInvM_ll_afterSelButInvMNJet_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);

	    if(met.p4.Et() > sel.GetMETCut().second) {  
	      if(sel.GetFlagb() == 0 || IsData){
		
		if( NBtaggedJets >=0 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup0Btag_ee, "HInvM_ll_afterSelButInvMSup0Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets >=1 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup1Btag_ee, "HInvM_ll_afterSelButInvMSup1Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets >=2 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup2Btag_ee, "HInvM_ll_afterSelButInvMSup2Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		
		if( NBtaggedJets ==0 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM0Btag_ee, "HInvM_ll_afterSelButInvM0Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets ==1 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM1Btag_ee, "HInvM_ll_afterSelButInvM1Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets ==2 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM2Btag_ee, "HInvM_ll_afterSelButInvM2Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);	        
	      }
	      if(sel.GetFlagb() == 1 && !IsData){
		
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup0Btag_ee, "HInvM_ll_afterSelButInvMSup0Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup1Btag_ee, "HInvM_ll_afterSelButInvMSup1Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*(1-weightb[1] ) );
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup2Btag_ee, "HInvM_ll_afterSelButInvMSup2Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[0]);		 
		
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM0Btag_ee, "HInvM_ll_afterSelButInvM0Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[1]);
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM1Btag_ee, "HInvM_ll_afterSelButInvM1Btag_ee", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[2]);
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM2Btag_ee, "HInvM_ll_afterSelButInvM2Btag_ee",InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[3]);  
		
	      }
	    }
	  }
	  
	}
	
	if( ChannelName=="emu" && CandType=="emu"){

        MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMDilepton_emu, "HInvM_ll_afterSelButInvMDilepton_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
        if(met.p4.Et() > sel.GetMETCut().second ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_emu,
	"HInvM_ll_afterSelButInvMNJet_AfterMET_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);	   
	double theweight = Dweight[ITypeMC];
	if(ITypeMC == 2) theweight = Dweight[ITypeMC]*1.38872;

        MyhistoManager.FillHisto(HNjets_emu_aftermetcutbutjetcut,  "Njets_emu_aftermetcutbutjetcut"  , JetMul, datasetName, IsSignal, theweight); 

	  if(theselJets.size()>=2 &&  InvDilMass> sel.GetMinValueMassCut() ){
	    MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMNJet_emu, "HInvM_ll_afterSelButInvMNJet_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	    
	    
	    if(met.p4.Et() > sel.GetMETCut().second) {  
	      if(sel.GetFlagb() == 0 || IsData){
		
		if( NBtaggedJets >=0 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup0Btag_emu, "HInvM_ll_afterSelButInvMSup0Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets >=1 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup1Btag_emu, "HInvM_ll_afterSelButInvMSup1Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets >=2 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup2Btag_emu, "HInvM_ll_afterSelButInvMSup2Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		
		if( NBtaggedJets ==0 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM0Btag_emu, "HInvM_ll_afterSelButInvM0Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets ==1 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM1Btag_emu, "HInvM_ll_afterSelButInvM1Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		if( NBtaggedJets ==2 ) MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM2Btag_emu, "HInvM_ll_afterSelButInvM2Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
	      }
	      if(sel.GetFlagb() == 1 && !IsData){
		
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup0Btag_emu, "HInvM_ll_afterSelButInvMSup0Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup1Btag_emu, "HInvM_ll_afterSelButInvMSup1Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*(1-weightb[1] ) );
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvMSup2Btag_emu, "HInvM_ll_afterSelButInvMSup2Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[0]);		    
		
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM0Btag_emu, "HInvM_ll_afterSelButInvM0Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[1]); 
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM1Btag_emu, "HInvM_ll_afterSelButInvM1Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[2]); 
		MyhistoManager.FillHisto(HInvM_ll_afterSelButInvM2Btag_emu, "HInvM_ll_afterSelButInvM2Btag_emu", InvDilMass,datasetName,IsSignal,Dweight[ITypeMC]*weightb[3]);   
		
	      }
	      
	      
	    }
	  }
	  
	}
	  
	  
	
    	
	
	int NBtaggedJets3 = NBtaggedJets;
	if (NBtaggedJets3>3) NBtaggedJets3 = 3; 
	
	if(CandType=="mumu" && ChannelName=="mumu" ){
	  MyhistoManager.FillHisto(HInvM_mumu_pair, "InvmumuMassPair" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HInvM_mumu_pairzoom, "InvmumuMassPairzoom" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HNjets_mumu,     "Njets_mumu"      ,JetMul,       datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HMet_mumu,       "Met_mumu"        ,misset,       datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HPtMu1_mumu, "PtMu1_mumu" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HPtMu2_mumu, "PtMu2_mumu" ,candMuon[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HNVxP_mumu, "NVxP_mumu" ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhiLMet_mumu, "DeltaPhiLMet_mumu" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	  if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu, "PtJet1_mumu" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu, "PtJet2_mumu" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  if (sel.GetFlagb() == 0 || IsData) {
	    MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	  }
	  else if (sel.GetFlagb() == 1) {
	    MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	  }
	}
	if(CandType=="ee" && ChannelName=="ee" ){
	  
	  
	  
	  if( (datasetName!="DataMuEG" && datasetName!="DataEG" &&  datasetName!="DataMu") || event->runNb > 163869){
	    if(candElec[0].isEB == 1  && candElec[1].isEB == 1 ) 
	      MyhistoManager.FillHisto(HInvM_ee_pair_EB, "HInvM_ee_pair_EB", InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	    if(candElec[0].isEB == 0  && candElec[1].isEB == 0 ) 
	      MyhistoManager.FillHisto(HInvM_ee_pair_EE, "HInvM_ee_pair_EE", InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	  }
	  
	  
    
	  
	  
	  
	  MyhistoManager.FillHisto(HInvM_ee_pair, "InveeMassPair" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HInvM_ee_pairzoom, "InveeMassPairzoom" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HNjets_ee,     "Njets_ee"      ,JetMul,       datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HMet_ee,       "Met_ee"        ,misset,       datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HPtEl1_ee, "PtEl1_ee" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HPtEl2_ee, "PtEl2_ee" ,candElec[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HNVxP_ee, "NVxP_ee" ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhiLMet_ee, "DeltaPhiLMet_ee" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	  if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee, "PtJet1_ee" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee, "PtJet2_ee" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  if (sel.GetFlagb() == 0 || IsData) {
	    MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	  }
	  else if (sel.GetFlagb() == 1) {
	    MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	  }
	}
	if(CandType=="emu" && ChannelName=="emu" ){
	  MyhistoManager.FillHisto(HInvM_emu_pair, "InvemuMassPair" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HInvM_emu_pairzoom, "InvemuMassPairzoom" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HNjets_emu,     "Njets_emu"      ,JetMul,       datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HMet_emu,       "Met_emu"        ,misset,       datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HNVxP_emu, "NVxP_emu" ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  MyhistoManager.FillHisto(HDeltaPhiLMet_emu, "DeltaPhiLMet_emu" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	  if (candElec[0].p4.Pt () > candMuon[0].p4.Pt ()) { 
	    MyhistoManager.FillHisto(HPtLpt1_emu, "PtLpt1_emu" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtLpt2_emu, "PtLpt2_emu" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  } else {
	    MyhistoManager.FillHisto(HPtLpt1_emu, "PtLpt1_emu" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtLpt2_emu, "PtLpt2_emu" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  }
	  if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_emu, "PtJet1_emu" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_emu, "PtJet2_emu" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	  if (sel.GetFlagb() == 0 || IsData) {
	    MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	  }
	  else if (sel.GetFlagb() == 1) {
	    MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	  }
	}
	
        
//DYEstimation : mumu
	if ( ChannelName=="mumu" && CandType == ChannelName ){
	  bool isinSR = false;
	  bool isinCR = false;
	  
	  float metcutForDY = sel.GetMETCut().second;
	  // cut au niveau dilepton pair ou apres la coupure en met
	  if ( !IDYestimateWithMetCut ) metcutForDY = -1;
	  
	  if ( theselJets.size()>1 ) {
	    if ( met.p4.Et()>metcutForDY ) {
              if ( ((event->TMEME==20)||(event->TMEME==11010)||(event->TMEME==22000)) )    MyhistoManager.FillHisto(hlistSR_mumu,"hlistSR_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
              if ( !(((event->TMEME==20)||(event->TMEME==11010)||(event->TMEME==22000))) ) MyhistoManager.FillHisto(hlistSR_mumu,"hlistSR_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	      isinSR = true;
	    }	 
	  }  
	  
	  if ( theselJets.size()<=1 ) { 
	    if ( met.p4.Et()> metcutForDY) { 
              if ( ((event->TMEME==20)||(event->TMEME==11010)||(event->TMEME==22000)) )    MyhistoManager.FillHisto(hlistCR_mumu,"hlistCR_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
              if ( !(((event->TMEME==20)||(event->TMEME==11010)||(event->TMEME==22000))) ) MyhistoManager.FillHisto(hlistCR_mumu,"hlistCR_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	      if (datasetName!="Zjets" && datasetName!="DYToMuMu_M-20" && 
		  datasetName!="DYToEE_M-20" && datasetName!="DYToTauTau_M-20" &&
		  datasetName!="DYToMuMu_M-10To20" && datasetName!="DYToEE_M-10To20" && 
		  datasetName!="DYToTauTau_M-10To20" &&datasetName!="DYToLL_M10-50" ) {
              if ( ((event->TMEME==20)||(event->TMEME==11010)||(event->TMEME==22000)) )    MyhistoManager.FillHisto(hlistContCR_mumu,"hlistContCR_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
              if ( !(((event->TMEME==20)||(event->TMEME==11010)||(event->TMEME==22000))) ) MyhistoManager.FillHisto(hlistContCR_mumu,"hlistContCR_mumu",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	      }
	      isinCR = true;
	    }
	  }

	  if ( met.p4.Et()>metcutForDY ){ 
             float DilM = sel.DiLeptonMass(candMuon,candElec);
             if ( isinCR ) { 
                MyhistoManager.FillHisto(DYMassCR_mumu,"DYMassCR_mumu",DilM,datasetName,IsSignal,Dweight[ITypeMC]);
	        if ( (DilM<76 || DilM>106) ) {
                   MyhistoManager.FillHisto(NjetsCR_outMZ_mumu,"NjetsCR_outMZ_mumu",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	        if ( (DilM>=76 && DilM<=106) ) {
                   MyhistoManager.FillHisto(NjetsCR_inMZ_mumu,"NjetsCR_inMZ_mumu",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	     } //isinCR 
             if ( isinSR ) { 
                MyhistoManager.FillHisto(DYMassSR_mumu,"DYMassSR_mumu",DilM,datasetName,IsSignal,Dweight[ITypeMC]);
	        if ( (DilM<76 || DilM>106) ) {
                   MyhistoManager.FillHisto(NjetsSR_outMZ_mumu,"NjetsSR_outMZ_mumu",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	        if ( (DilM>=76 && DilM<=106) ) {
                   MyhistoManager.FillHisto(NjetsSR_inMZ_mumu,"NjetsSR_inMZ_mumu",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	     } //isinSR 
	   } // (eventual) met cut
	}  // mumu


//DYEstimation : ee
	if ( ChannelName=="ee"  && CandType == ChannelName ){
              bool isinSR = false;
              bool isinCR = false;
	      
	  float metcutForDY = sel.GetMETCut().second;
	  // cut au niveau dilepton pair ou apres la coupure en met
	  if ( !IDYestimateWithMetCut ) metcutForDY = -1;
	  
	  if ( theselJets.size()>1 ) {
	    if ( met.p4.Et()>metcutForDY ) {
              if ( ((event->TMEME==2)||(event->TMEME==10101)||(event->TMEME==20200)) )    MyhistoManager.FillHisto(hlistSR_ee,"hlistSR_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
              if ( !((event->TMEME==2)||(event->TMEME==10101)||(event->TMEME==20200)) )   MyhistoManager.FillHisto(hlistSR_ee,"hlistSR_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	      isinSR = true;
	    }	 
	  }  
	  
	  if ( theselJets.size()<=1 ) { 
	    if ( met.p4.Et()> metcutForDY) { 
              if ( ((event->TMEME==2)||(event->TMEME==10101)||(event->TMEME==20200)) )    MyhistoManager.FillHisto(hlistCR_ee,"hlistCR_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
              if ( !((event->TMEME==2)||(event->TMEME==10101)||(event->TMEME==20200)) )   MyhistoManager.FillHisto(hlistCR_ee,"hlistCR_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	      if (datasetName!="Zjets" && datasetName!="DYToMuMu_M-20" && 
		  datasetName!="DYToEE_M-20" && datasetName!="DYToTauTau_M-20" &&
		  datasetName!="DYToMuMu_M-10To20" && datasetName!="DYToEE_M-10To20" && 
		  datasetName!="DYToTauTau_M-10To20" && datasetName!="DYToLL_M10-50") {
              if ( ((event->TMEME==2)||(event->TMEME==10101)||(event->TMEME==20200)) )    MyhistoManager.FillHisto(hlistContCR_ee,"hlistContCR_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,true,Dweight[ITypeMC]);	      
              if ( !((event->TMEME==2)||(event->TMEME==10101)||(event->TMEME==20200)) )   MyhistoManager.FillHisto(hlistContCR_ee,"hlistContCR_ee",sel.DiLeptonMass(candMuon,candElec),datasetName,false,Dweight[ITypeMC]);	      
	      }
	      isinCR = true;
	    }
	  }

	  if ( met.p4.Et()>metcutForDY ){ 
             float DilM = sel.DiLeptonMass(candMuon,candElec);
             if ( isinCR ) { 
                MyhistoManager.FillHisto(DYMassCR_ee,"DYMassCR_ee",DilM,datasetName,IsSignal,Dweight[ITypeMC]);
	        if ( (DilM<76 || DilM>106) ) {
                   MyhistoManager.FillHisto(NjetsCR_outMZ_ee,"NjetsCR_outMZ_ee",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	        if ( (DilM>=76 && DilM<=106) ) {
                   MyhistoManager.FillHisto(NjetsCR_inMZ_ee,"NjetsCR_inMZ_ee",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	     } //isinCR
             if ( isinSR ) { 
                MyhistoManager.FillHisto(DYMassSR_ee,"DYMassSR_ee",DilM,datasetName,IsSignal,Dweight[ITypeMC]);
	        if ( (DilM<76 || DilM>106) ) {
                   MyhistoManager.FillHisto(NjetsSR_outMZ_ee,"NjetsSR_outMZ_ee",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	        if ( (DilM>=76 && DilM<=106) ) {
                   MyhistoManager.FillHisto(NjetsSR_inMZ_ee,"NjetsSR_inMZ_ee",theselJets.size(),datasetName,IsSignal,Dweight[ITypeMC]);
                }
	     } //isinSR 
	   }
	} //ee
//EndDYEstimation	
	
	// Rough Z->tautau->emu selection
	if ( theselJets.size()<2 )  {
	        if(CandType=="emu" && ChannelName=="emu"){
		  if (datasetName!="Zjets" ) {
		    MyhistoManager.FillHisto(HInvM_emu_pair_Zemu, "InvemuMassPair_Zemu" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
                   }else {
		    if (event->TMEME==21100) MyhistoManager.FillHisto(HInvM_emu_pair_Zemu, "InvemuMassPair_Zemu" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  }
	        }
	}
	
	// Passage invariante mass dilepton
	//	  if (( InvDilMass<sel.GetZmassWindowCut().first || InvDilMass>sel.GetZmassWindowCut().second ) && (InvDilMass> sel.GetMinValueMassCut() ))  {
	if ( ( (ChannelName=="mumu" || ChannelName=="ee") && 
	       sel.DiLeptonMassCut(sel.GetMinValueMassCut(),sel.GetZmassWindowCut(),candMuon,candElec,ChannelName) ) || 
		(ChannelName=="emu" && InvDilMass> sel.GetMinValueMassCut())
	     )  {
	     
	   
	   //**************************************
	   //implement uncertainty on events yield.
	   //**************************************
	     
	   // APPLY SF_DD HERE! :
	   /*if (ChannelName=="mumu"){ 
	     if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { 
	       Dweight[ITypeMC]*=vSF_DY_mm[0]; 
	       EventYieldWeightError = vSF_DY_mm_error[0]*Dweight[ITypeMC];}
	   }
	   else if (ChannelName=="emu" ){
	     if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { 
	       Dweight[ITypeMC]*=vSF_DY_em[0]; 
	       EventYieldWeightError =  vSF_DY_em_error[0]*Dweight[ITypeMC];}
	   }
	   else if (ChannelName=="ee"  ){
	     if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { Dweight[ITypeMC]*=vSF_DY_ee[0]; 
	     EventYieldWeightError = vSF_DY_ee_error[0]*Dweight[ITypeMC];}
	   }
	   */
	   
	   
	   //cout << "EventYieldWeightError 2036 " << EventYieldWeightError  << endl;
	   
	  //TabFlow1[IChannel][ITypeMC][3]+=Dweight[ITypeMC];
	  //TabFlow2[IChannel][ITypeMC][3]+=EventYieldWeightError;
	  if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 3, datasetName, IsSignal, Dweight[ITypeMC]);
	  if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 3, datasetName, IsSignal, Dweight[ITypeMC]);
	  if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 3, datasetName, IsSignal, Dweight[ITypeMC]);
    	  if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 3, datasetName, IsSignal, EventYieldWeightError);
 	  if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 3, datasetName, IsSignal, EventYieldWeightError);
 	  if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 3, datasetName, IsSignal, EventYieldWeightError);
	  
	  fHist->Fill(3.,Dweight[ITypeMC] );
	  
	  

	   
          if (debugcc<maxdebugcc) {
            cout << " selmass " ;
	  }
	  
	  if(CandType=="mumu" && ChannelName=="mumu"){ 
	    MyhistoManager.FillHisto(HInvM_mumu_pair_afterveto, "InvmumuMassPair_afterveto" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNjets_mumu_afterveto,     "Njets_mumu_afterveto"     ,JetMul,datasetName       , IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HMet_mumu_afterveto,       "Met_mumu_afterveto"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtMu1_mumu_afterveto, "PtMu1_mumu_afterveto" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtMu2_mumu_afterveto, "PtMu2_mumu_afterveto" ,candMuon[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HDeltaPhiLMet_mumu_afterveto, "DeltaPhiLMet_mumu_afterveto" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu_afterveto, "PtJet1_mumu_afterveto" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu_afterveto, "PtJet2_mumu_afterveto" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    if (sel.GetFlagb() == 0 || IsData) {
	      MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	    }
	    else if (sel.GetFlagb() == 1) {
	      MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	    }
	  }
	  if(CandType=="ee" && ChannelName=="ee"){
	    MyhistoManager.FillHisto(HInvM_ee_pair_afterveto, "InveeMassPair_afterveto" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNjets_ee_afterveto,     "Njets_ee_afterveto"      ,JetMul,       datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HMet_ee_afterveto,       "Met_ee_afterveto"        ,misset,       datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtEl1_ee_afterveto, "PtEl1_ee_afterveto" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtEl2_ee_afterveto, "PtEl2_ee_afterveto" ,candElec[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HDeltaPhiLMet_ee_afterveto, "DeltaPhiLMet_ee_afterveto" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee_afterveto, "PtJet1_ee_afterveto" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee_afterveto, "PtJet2_ee_afterveto" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	    if (sel.GetFlagb() == 0 || IsData) {
	      MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	    }
	    else if (sel.GetFlagb() == 1) {
	      MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	    }
	  }
	  
	  // Passage multiplicite de jets
	  if ( theselJets.size()>=2 )  {
	    //TabFlow1[IChannel][ITypeMC][4]+=Dweight[ITypeMC];
	    //TabFlow2[IChannel][ITypeMC][4]+=EventYieldWeightError;
	    
	    // APPLY SF_DD HERE! :
	    
	    //double EventYieldWeightError = EventYieldWeightError;
	    if (ChannelName=="mumu"){ 
	      if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 ){ 
	        Dweight[ITypeMC]*=vSF_DY_mm[1]; 
		EventYieldWeightError = (vSF_DY_mm_error[1]*initWeight);}
	    }
	    else if (ChannelName=="emu" ){
	      if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 ){ 
	        Dweight[ITypeMC]*=vSF_DY_em[1]; 
		EventYieldWeightError = (vSF_DY_em_error[1]*initWeight); }
	    }
	    else if (ChannelName=="ee"  ){
	      if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 ){ 
	        Dweight[ITypeMC]*=vSF_DY_ee[1]; 
		EventYieldWeightError = (vSF_DY_ee_error[1]*initWeight); }
	    }
	    
	    //cout << "EventYieldWeightError 2118 " << EventYieldWeightError << endl;
	    
	    if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 4, datasetName, IsSignal, Dweight[ITypeMC]);
	    if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 4, datasetName, IsSignal, Dweight[ITypeMC]);
	    if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 4, datasetName, IsSignal, Dweight[ITypeMC]);
	    
	    
 	    if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 4, datasetName, IsSignal, EventYieldWeightError);
 	    if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 4, datasetName, IsSignal, EventYieldWeightError);
 	    if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 4, datasetName, IsSignal, EventYieldWeightError);
	    
	    
 	    fHist->Fill(4.,Dweight[ITypeMC] );
	    
	    
	    if (debugcc<maxdebugcc) {
            cout << " seljet " ;
	    }
	    
	    
    
	    if(CandType=="mumu" && ChannelName=="mumu"){ 
	      MyhistoManager.FillHisto(HInvM_mumu_pair_afterjetcut, "InvmumuMassPair_afterjetcut" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNjets_mumu_afterjetcut,     "Njets_mumu_afterjetcut"     ,JetMul,datasetName       , IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HMet_mumu_afterjetcut,       "Met_mumu_afterjetcut"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HPtMu1_mumu_afterjetcut, "PtMu1_mumu_afterjetcut" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HPtMu2_mumu_afterjetcut, "PtMu2_mumu_afterjetcut" ,candMuon[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HDeltaPhiLMet_mumu_afterjetcut, "DeltaPhiLMet_mumu_afterjetcut" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	      if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu_afterjetcut, "PtJet1_mumu_afterjetcut" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu_afterjetcut, "PtJet2_mumu_afterjetcut" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      if (sel.GetFlagb() == 0 || IsData) {
		MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	      }
	      else if (sel.GetFlagb() == 1) {
		MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	      }
	    }
	    if(CandType=="ee" && ChannelName=="ee"){
	      MyhistoManager.FillHisto(HInvM_ee_pair_afterjetcut, "InveeMassPair_afterjetcut" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNjets_ee_afterjetcut,     "Njets_ee_afterjetcut"      ,JetMul,       datasetName, IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HMet_ee_afterjetcut,       "Met_ee_afterjetcut"        ,misset,       datasetName, IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HPtEl1_ee_afterjetcut, "PtEl1_ee_afterjetcut" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HPtEl2_ee_afterjetcut, "PtEl2_ee_afterjetcut" ,candElec[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HDeltaPhiLMet_ee_afterjetcut, "DeltaPhiLMet_ee_afterjetcut" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	      if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee_afterjetcut, "PtJet1_ee_afterjetcut" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee_afterjetcut, "PtJet2_ee_afterjetcut" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      if (sel.GetFlagb() == 0 || IsData) {
		MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	      }
	      else if (sel.GetFlagb() == 1) {
		MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	      }
	    }
	    if(CandType=="emu" && ChannelName=="emu"){
	      MyhistoManager.FillHisto(HInvM_emu_pair_afterjetcut, "InvemuMassPair_afterjetcut" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNjets_emu_afterjetcut,     "Njets_emu_afterjetcut"     ,JetMul,datasetName       , IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HMet_emu_afterjetcut,       "Met_emu_afterjetcut"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HDeltaPhiLMet_emu_afterjetcut, "DeltaPhiLMet_emu_afterjetcut" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
	      if (candElec[0].p4.Pt () > candMuon[0].p4.Pt ()) { 
		MyhistoManager.FillHisto(HPtLpt1_emu_afterjetcut, "PtLpt1_emu_afterjetcut" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HPtLpt2_emu_afterjetcut, "PtLpt2_emu_afterjetcut" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      } else {
		MyhistoManager.FillHisto(HPtLpt1_emu_afterjetcut, "PtLpt1_emu_afterjetcut" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HPtLpt2_emu_afterjetcut, "PtLpt2_emu_afterjetcut" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      }
	      if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_emu_afterjetcut, "PtJet1_emu_afterjetcut" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_emu_afterjetcut, "PtJet2_emu_afterjetcut" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
	      if (sel.GetFlagb() == 0 || IsData) {
		MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
	      }
	      else if (sel.GetFlagb() == 1) {
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]);
	      }
	    }
	    
	    // plot MET after btag but w/o MET cut
	    if (sel.GetFlagb() == 0 || IsData) {
	      if (NBtaggedJets>=1 ) {
		if(CandType=="mumu" && ChannelName=="mumu"){
		  MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag1, "InvmumuMassPair_afterbtag1" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HMet_mumu_afterbtag1,       "Met_mumu_afterbtag1"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		}
		else if(CandType=="ee" && ChannelName=="ee"){
		  MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag1, "InveeMassPair_afterbtag1" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HMet_ee_afterbtag1,         "Met_ee_afterbtag1"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		}
		else if(CandType=="emu" && ChannelName=="emu"){
		  MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag1, "InvemuMassPair_afterbtag1" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HMet_emu_afterbtag1,        "Met_emu_afterbtag1"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		}
	      }
	      if (NBtaggedJets>=2 ) {
		if(CandType=="mumu" && ChannelName=="mumu"){
		  MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag2, "InvmumuMassPair_afterbtag2" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HMet_mumu_afterbtag2,       "Met_mumu_afterbtag2"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		}
		else if(CandType=="ee" && ChannelName=="ee"){
		  MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag2, "InveeMassPair_afterbtag2" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HMet_ee_afterbtag2,         "Met_ee_afterbtag2"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		}
		else if(CandType=="emu" && ChannelName=="emu"){
		  MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag2, "InvemuMassPair_afterbtag2" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HMet_emu_afterbtag2,        "Met_emu_afterbtag2"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		}
	      }
	    }
	    else if (sel.GetFlagb() == 1) {
	      if(CandType=="mumu" && ChannelName=="mumu"){
		MyhistoManager.FillHisto(HMet_mumu_afterbtag1,       "Met_mumu_afterbtag1"       ,misset,datasetName       , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HMet_mumu_afterbtag2,       "Met_mumu_afterbtag2"       ,misset,datasetName       , IsSignal, weightb[0]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag1, "InvmumuMassPair_afterbtag1" ,InvDilMass,datasetName   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag2, "InvmumuMassPair_afterbtag2" ,InvDilMass,datasetName   , IsSignal, weightb[0]*Dweight[ITypeMC]);
	      }
	      else if(CandType=="ee" && ChannelName=="ee"){
		MyhistoManager.FillHisto(HMet_ee_afterbtag1,         "Met_ee_afterbtag1"       ,misset,datasetName       , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HMet_ee_afterbtag2,         "Met_ee_afterbtag2"       ,misset,datasetName       , IsSignal, weightb[0]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag1, "InveeMassPair_afterbtag1" ,InvDilMass,datasetName   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag2, "InveeMassPair_afterbtag2" ,InvDilMass,datasetName   , IsSignal, weightb[0]*Dweight[ITypeMC]);
	      }
	      else if(CandType=="emu" && ChannelName=="emu"){
		MyhistoManager.FillHisto(HMet_emu_afterbtag1,        "Met_emu_afterbtag1"       ,misset,datasetName       , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HMet_emu_afterbtag2,        "Met_emu_afterbtag2"       ,misset,datasetName       , IsSignal, weightb[0]*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag1, "InvemuMassPair_afterbtag1" ,InvDilMass,datasetName   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag2, "InvemuMassPair_afterbtag2" ,InvDilMass,datasetName   , IsSignal, weightb[0]*Dweight[ITypeMC]);
		
	      } 
	    }
	    
	    // Passage met
	    if ( ((ChannelName=="mumu" || ChannelName=="ee") &&  met.p4.Et()>sel.GetMETCut().second) ||
		 (ChannelName=="emu"  &&  met.p4.Et()>sel.GetMETCut().first)    )  {
	      // APPLY SF_met HERE! :
	      
	      
	      if( datasetName=="TTbar" && IsSignal ){
	      
	        if (ChannelName=="mumu"){ 
		  Dweight[ITypeMC]*= SF_met_mumu;
	          EventYieldWeightError = Dweight[ITypeMC]*Dweight[ITypeMC] ;
		}
	        if (ChannelName=="emu"){ 
		  Dweight[ITypeMC]*= SF_met_emu;
	          EventYieldWeightError =  Dweight[ITypeMC]*Dweight[ITypeMC]  ;
		}
	        if (ChannelName=="ee"){ 
		  Dweight[ITypeMC]*= SF_met_ee;
	          EventYieldWeightError =  Dweight[ITypeMC]*Dweight[ITypeMC] ;
		}
		 
	     }
	      

	      
	      // APPLY SF_DD HERE! :
	      if (ChannelName=="mumu"){ 	  
		
		if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )   { 
		  Dweight[ITypeMC]*=vSF_DY_mm[2]; 
		  EventYieldWeightError = vSF_DY_mm_error[2]*initWeight;
		  }
	      }
	      else if (ChannelName=="emu" ){
		if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { 
		  Dweight[ITypeMC]*=vSF_DY_em[2];
		  EventYieldWeightError = (vSF_DY_em_error[2]*initWeight); }
	      }
	      else if (ChannelName=="ee"  ){
		if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { 
		  Dweight[ITypeMC]*=vSF_DY_ee[2];
		  EventYieldWeightError = (vSF_DY_ee_error[2]*initWeight); }
	      }
	      
	      //cout << "EventYieldWeightError 2287" << EventYieldWeightError << endl;
	      
	      // APPLY SF_DD HERE! :
	      
	      double weight_SFDY_1btag[3] = {1,1,1};
	      double weight_SFDY_2btag[3] = {1,1,1};
	      
	      double weight_SFfakeBck_1btag[3] = {1,1,1};
	      double weight_SFfakeBck_2btag[3] = {1,1,1};
	      
	
	
	      double weight_SFtotal_1btag[3] = {1,1,1};
	      double weight_SFtotal_2btag[3] = {1,1,1};
	      
	      double EventYieldWeightError_Sup1btag = (1-weightb[1])*Dweight[ITypeMC]*(1-weightb[1])*Dweight[ITypeMC];
	      double EventYieldWeightError_Sup2btag = weightb[0]*Dweight[ITypeMC]*weightb[0]*Dweight[ITypeMC];

	      if (ChannelName=="mumu"){ 
	        if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )	{ 
		  weight_SFDY_1btag[0]=vSF_DY_mm[3]; 
		  weight_SFDY_2btag[0]=vSF_DY_mm[4];
		  EventYieldWeightError_Sup1btag = (1-weightb[1])*initWeight*vSF_DY_mm_error[3];
		  EventYieldWeightError_Sup2btag = weightb[0]*initWeight*vSF_DY_mm_error[4];
		}else if (isttbar && !IsSignal  ) { 
		  weight_SFfakeBck_1btag[0]=vSF_FakeBck_mm[1]; 
		  weight_SFfakeBck_2btag[0]=vSF_FakeBck_mm[2];
		  EventYieldWeightError_Sup1btag = (1-weightb[1])*initWeight*vSF_FakeBck_mm_error[1];
		  EventYieldWeightError_Sup2btag = weightb[0]*initWeight*vSF_FakeBck_mm_error[2];
		 }
	      }
	      else if (ChannelName=="emu" ){
	        if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )	{ 
		  weight_SFDY_1btag[1]=vSF_DY_em[3]; 
		  weight_SFDY_2btag[1]=vSF_DY_em[4]; 
		  EventYieldWeightError_Sup1btag = (1-weightb[1])*initWeight*vSF_DY_em_error[3];
		  EventYieldWeightError_Sup2btag = weightb[0]*initWeight*vSF_DY_em_error[4];
	        }else if (isttbar && !IsSignal ) { 
		  weight_SFfakeBck_1btag[1]=vSF_FakeBck_em[1]; 
		  weight_SFfakeBck_2btag[1]=vSF_FakeBck_em[2];
		  EventYieldWeightError_Sup1btag = (1-weightb[1])*initWeight*vSF_FakeBck_em_error[1];
		  EventYieldWeightError_Sup2btag = weightb[0]*initWeight*vSF_FakeBck_em_error[2];
		}
	      }
	      else if (ChannelName=="ee"  ){
	        if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )	{ 
		  weight_SFDY_1btag[2]=vSF_DY_ee[3]; 
		  weight_SFDY_2btag[2]=vSF_DY_ee[4];
		  EventYieldWeightError_Sup1btag = (1-weightb[1])*initWeight*vSF_DY_ee_error[3];
		  EventYieldWeightError_Sup2btag = weightb[0]*initWeight*vSF_DY_ee_error[4];
		}else if (isttbar && !IsSignal ) { 
		  weight_SFfakeBck_1btag[2]=vSF_FakeBck_ee[1]; 
		  weight_SFfakeBck_2btag[2]=vSF_FakeBck_ee[2];
		  EventYieldWeightError_Sup1btag = (1-weightb[1])*initWeight*vSF_FakeBck_ee_error[1];
		  EventYieldWeightError_Sup2btag = weightb[0]*initWeight*vSF_FakeBck_ee_error[2];
		}
	      }
	      
              weight_SFtotal_1btag[0] =  weight_SFDY_1btag[0] * weight_SFfakeBck_1btag[0]  ;
              weight_SFtotal_1btag[1] =  weight_SFDY_1btag[1] * weight_SFfakeBck_1btag[1]  ;
              weight_SFtotal_1btag[2] =  weight_SFDY_1btag[2] * weight_SFfakeBck_1btag[2]  ;
	      
	      
              weight_SFtotal_2btag[0] =  weight_SFDY_2btag[0] * weight_SFfakeBck_2btag[0]  ;
              weight_SFtotal_2btag[1] =  weight_SFDY_2btag[1] * weight_SFfakeBck_2btag[1]  ;
              weight_SFtotal_2btag[2] =  weight_SFDY_2btag[2] * weight_SFfakeBck_2btag[2]  ;
	      
	      
	      
	      
	      if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 5, datasetName, IsSignal, Dweight[ITypeMC]);
	      if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 5, datasetName, IsSignal, Dweight[ITypeMC]);
	      if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 5, datasetName, IsSignal, Dweight[ITypeMC]);
 	      if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 5, datasetName, IsSignal, EventYieldWeightError);
 	      if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 5, datasetName, IsSignal, EventYieldWeightError);
 	      if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 5, datasetName, IsSignal, EventYieldWeightError);
	      fHist->Fill(5.,Dweight[ITypeMC] );
	      
	      //TabFlow1[IChannel][ITypeMC][5]+=Dweight[ITypeMC];
	      //TabFlow2[IChannel][ITypeMC][5]+=EventYieldWeightError;
	      
	      if (debugcc<maxdebugcc) {
		cout << " selmet " ;
	      }
	      
	      //make sense?
//		if(checkDuplicate[event->eventNb] == event->runNb) cout << "duplicate evt" << event->eventNb << "  run " << event->runNb << endl;
	      
	      
	      if(CandType=="mumu" && ChannelName=="mumu"){ 
		MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetcut, "InvmumuMassPair_aftermetcut" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNjets_mumu_aftermetcut,     "Njets_mumu_aftermetcut"     ,JetMul,datasetName       , IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HMet_mumu_aftermetcut,       "Met_mumu_aftermetcut"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HPtMu1_mumu_aftermetcut, "PtMu1_mumu_aftermetcut" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HPtMu2_mumu_aftermetcut, "PtMu2_mumu_aftermetcut" ,candMuon[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNVxP_mumu_aftermetcut, "NVxP_mumu_aftermetcut" ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HDeltaPhiLMet_mumu_aftermetcut, "DeltaPhiLMet_mumu_aftermetcut" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
		if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu_aftermetcut, "PtJet1_mumu_aftermetcut" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu_aftermetcut, "PtJet2_mumu_aftermetcut" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		if (sel.GetFlagb() == 0 || IsData) {
		  MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
		}
		else if (sel.GetFlagb() == 1) {
		  MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]*weight_SFtotal_1btag[0]);
		  MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]*weight_SFtotal_2btag[0]);
		  MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]*weight_SFtotal_2btag[0]);
		}
	      }
	      if(CandType=="ee" && ChannelName=="ee"){
		MyhistoManager.FillHisto(HInvM_ee_pair_aftermetcut, "InveeMassPair_aftermetcut" ,InvDilMass,   datasetName, IsSignal, Dweight[ITypeMC]);
		//                  std::cout<<"code reference after "<<Dweight[ITypeMC] <<std::endl;
		MyhistoManager.FillHisto(HNjets_ee_aftermetcut,     "Njets_ee_aftermetcut"      ,JetMul,       datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HMet_ee_aftermetcut,       "Met_ee_aftermetcut"        ,misset,       datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HPtEl1_ee_aftermetcut, "PtEl1_ee_aftermetcut" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HPtEl2_ee_aftermetcut, "PtEl2_ee_aftermetcut" ,candElec[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNVxP_ee_aftermetcut, "NVxP_ee_aftermetcut" ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HDeltaPhiLMet_ee_aftermetcut, "DeltaPhiLMet_ee_aftermetcut" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
		if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee_aftermetcut, "PtJet1_ee_aftermetcut" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee_aftermetcut, "PtJet2_ee_aftermetcut" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		if (sel.GetFlagb() == 0 || IsData) {
		  MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
		}
		else if (sel.GetFlagb() == 1) {
		  MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]*weight_SFtotal_1btag[2]);
		  MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]*weight_SFtotal_2btag[2]);
		  MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]*weight_SFtotal_2btag[2]);
		}
	      }
	      if(CandType=="emu" && ChannelName=="emu"){
		MyhistoManager.FillHisto(HInvM_emu_pair_aftermetcut, "InvemuMassPair_aftermetcut" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNjets_emu_aftermetcut,     "Njets_emu_aftermetcut"     ,JetMul,datasetName       , IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HMet_emu_aftermetcut,       "Met_emu_aftermetcut"       ,misset,datasetName       , IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HNVxP_emu_aftermetcut, "NVxP_emu_aftermetcut" ,selVertices.size(),   datasetName, IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(HDeltaPhiLMet_emu_aftermetcut, "DeltaPhiLMet_emu_aftermetcut" ,deltaPhi_met,   datasetName, IsSignal, Dweight[ITypeMC]);
		if (candElec[0].p4.Pt () > candMuon[0].p4.Pt ()) { 
		  MyhistoManager.FillHisto(HPtLpt1_emu_aftermetcut, "PtLpt1_emu_aftermetcut" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HPtLpt2_emu_aftermetcut, "PtLpt2_emu_aftermetcut" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		} else {
		  MyhistoManager.FillHisto(HPtLpt1_emu_aftermetcut, "PtLpt1_emu_aftermetcut" ,candMuon[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HPtLpt2_emu_aftermetcut, "PtLpt2_emu_aftermetcut" ,candElec[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		}
		if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_emu_aftermetcut, "PtJet1_emu_aftermetcut" ,theselJets[0].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_emu_aftermetcut, "PtJet2_emu_aftermetcut" ,theselJets[1].p4.Pt(),   datasetName, IsSignal, Dweight[ITypeMC]);
		if (sel.GetFlagb() == 0 || IsData) {
		  MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,NBtaggedJets3,datasetName, IsSignal, Dweight[ITypeMC]);
		}
		else if (sel.GetFlagb() == 1) {
		  MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,0,datasetName, IsSignal, weightb[1]*Dweight[ITypeMC]);
		  MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,1,datasetName, IsSignal, weightb[2]*Dweight[ITypeMC]*weight_SFtotal_1btag[1]);
		  MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,2,datasetName, IsSignal, weightb[3]*Dweight[ITypeMC]*weight_SFtotal_2btag[1]);
		  MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,3,datasetName, IsSignal, weightb[4]*Dweight[ITypeMC]*weight_SFtotal_2btag[1]);
                 }
	      }
	      
	      

	      // Passage Btagging
	      
	      double HT = 0;
	      for(unsigned int i=0; i<candElec.size(); i++){
		HT+=candElec[i].p4.Pt();
	      }
	      for(unsigned int i=0; i<candMuon.size(); i++){
		HT+=candMuon[i].p4.Pt();
	      }
	      for(unsigned int i=0; i<theselJets.size(); i++){
		HT+=theselJets[i].p4.Pt();
	      }
	      HT+=met.p4.Et();
	      
		
   
	      if (sel.GetFlagb() == 0 || IsData) {
		if ( NBtaggedJets>=1 )  {
		  //TabFlow1[IChannel][ITypeMC][6]+=Dweight[ITypeMC];
		  //TabFlow2[IChannel][ITypeMC][6]+=EventYieldWeightError;
		  
		  if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 6, datasetName, IsSignal, Dweight[ITypeMC]);
		  if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 6, datasetName, IsSignal, Dweight[ITypeMC]);
		  if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 6, datasetName, IsSignal, Dweight[ITypeMC]);
 	          if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 6, datasetName, IsSignal, EventYieldWeightError);
 	          if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 6, datasetName, IsSignal, EventYieldWeightError);
 	          if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 6, datasetName, IsSignal, EventYieldWeightError);
		  fHist->Fill(6.,Dweight[ITypeMC] );
		  
		  
		  
		  if(CandType=="ee"   && ChannelName=="ee"  ) {
		    MyhistoManager.FillHisto(HHT_ee_aftermetbtag1,   "HHT_ee_aftermetbtag1",   HT,datasetName, IsSignal, Dweight[ITypeMC]);
		    MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag1, "InveeMassPair_aftermetbtag1" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  }
		  else if(CandType=="emu"  && ChannelName=="emu" )  {
		    MyhistoManager.FillHisto(HHT_emu_aftermetbtag1,  "HHT_emu_aftermetbtag1",  HT,datasetName, IsSignal, Dweight[ITypeMC]);
		    MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag1, "InvemuMassPair_aftermetbtag1" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  }
		  else if(CandType=="mumu" && ChannelName=="mumu") {
		    MyhistoManager.FillHisto(HHT_mumu_aftermetbtag1, "HHT_mumu_aftermetbtag1", HT,datasetName, IsSignal, Dweight[ITypeMC]);
		    MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag1, "InvmumuMassPair_aftermetbtag1" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  }
		}
		if ( NBtaggedJets>=2 )  {
		  //TabFlow1[IChannel][ITypeMC][7]+=Dweight[ITypeMC];
		  //TabFlow2[IChannel][ITypeMC][7]+=EventYieldWeightError;
		  
		  
		  if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 7, datasetName, IsSignal, Dweight[ITypeMC]);
		  if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 7, datasetName, IsSignal, Dweight[ITypeMC]);
		  if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 7, datasetName, IsSignal, Dweight[ITypeMC]);
 	          if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 7, datasetName, IsSignal, EventYieldWeightError);
 	          if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 7, datasetName, IsSignal, EventYieldWeightError);
 	          if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 7, datasetName, IsSignal, EventYieldWeightError);
		  fHist->Fill(7.,Dweight[ITypeMC] );
		  
		  
		  if(CandType=="ee"   && ChannelName=="ee"  ) {
		    MyhistoManager.FillHisto(HHT_ee_aftermetbtag2,   "HHT_ee_aftermetbtag2",   HT,datasetName, IsSignal, Dweight[ITypeMC]);
		    MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag2, "InveeMassPair_aftermetbtag2" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  }
		  if(CandType=="emu"  && ChannelName=="emu" ) {
		    MyhistoManager.FillHisto(HHT_emu_aftermetbtag2,  "HHT_emu_aftermetbtag2",  HT,datasetName, IsSignal, Dweight[ITypeMC]);
		    MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag2, "InvemuMassPair_aftermetbtag2" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  }
		  if(CandType=="mumu" && ChannelName=="mumu") {
		    MyhistoManager.FillHisto(HHT_mumu_aftermetbtag2, "HHT_mumu_aftermetbtag2", HT,datasetName, IsSignal, Dweight[ITypeMC]);
		    MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag2, "InvmumuMassPair_aftermetbtag2" ,InvDilMass,datasetName   , IsSignal, Dweight[ITypeMC]);
		  }
		}
	      }
	      else if (sel.GetFlagb() == 1) {
	      
	      
	      // APPLY SF_DD HERE! :
	         
		 
		
		 
		 
		 
		//n btagged jet==1
 		  if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 6, datasetName, IsSignal, (1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[0]);
		  if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 6, datasetName, IsSignal, (1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[2]);
		  if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 6, datasetName, IsSignal, (1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[1]);
 	          if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 6, datasetName, IsSignal, EventYieldWeightError_Sup1btag );
 	          if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 6, datasetName, IsSignal, EventYieldWeightError_Sup1btag );
 	          if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 6, datasetName, IsSignal, EventYieldWeightError_Sup1btag );

		//n btagged jet==2
 		  if(IChannel == 0) MyhistoManager.FillHisto(CutFlow_mumu, "CutFlow_mumu", 7, datasetName, IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[0]);
		  if(IChannel == 1) MyhistoManager.FillHisto(CutFlow_ee,   "CutFlow_ee"  , 7, datasetName, IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[2]);
		  if(IChannel == 2) MyhistoManager.FillHisto(CutFlow_emu,  "CutFlow_emu" , 7, datasetName, IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[1]);
 	          if(IChannel == 0) MyhistoManager.FillHisto(ErrCutFlow_mumu, "ErrCutFlow_mumu", 7, datasetName, IsSignal, EventYieldWeightError_Sup2btag);
 	          if(IChannel == 1) MyhistoManager.FillHisto(ErrCutFlow_ee,   "ErrCutFlow_ee"  , 7, datasetName, IsSignal, EventYieldWeightError_Sup2btag);
 	          if(IChannel == 2) MyhistoManager.FillHisto(ErrCutFlow_emu,  "ErrCutFlow_emu" , 7, datasetName, IsSignal, EventYieldWeightError_Sup2btag);

		
		if(CandType=="ee"   && ChannelName=="ee"  ) {
		  MyhistoManager.FillHisto(HHT_ee_aftermetbtag1,   "HHT_ee_aftermetbtag1",   HT,datasetName, IsSignal, (1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[2]);
		  MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag1, "InveeMassPair_aftermetbtag1" ,InvDilMass,datasetName   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[2]);
		  
		  MyhistoManager.FillHisto(HHT_ee_aftermetbtag2,   "HHT_ee_aftermetbtag2",   HT,datasetName, IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[2]);
		  MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag2, "InveeMassPair_aftermetbtag2" ,InvDilMass,datasetName   , IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[2]);
		}
		else if(CandType=="emu"  && ChannelName=="emu" ) {
		  MyhistoManager.FillHisto(HHT_emu_aftermetbtag1,  "HHT_emu_aftermetbtag1",  HT,datasetName, IsSignal,(1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[1]); 
		  MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag1, "InvemuMassPair_aftermetbtag1" ,InvDilMass,datasetName   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[1]);
		  
		  MyhistoManager.FillHisto(HHT_emu_aftermetbtag2,  "HHT_emu_aftermetbtag2",  HT,datasetName, IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[1]);
		  MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag2, "InvemuMassPair_aftermetbtag2" ,InvDilMass,datasetName   , IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[1]);
		}
		else if(CandType=="mumu" && ChannelName=="mumu") {
		  MyhistoManager.FillHisto(HHT_mumu_aftermetbtag1, "HHT_mumu_aftermetbtag1", HT,datasetName, IsSignal,(1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[0]); 
		  MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag1, "InvmumuMassPair_aftermetbtag1" ,InvDilMass,datasetName   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]*weight_SFtotal_1btag[0]);
		  
		  MyhistoManager.FillHisto(HHT_mumu_aftermetbtag2, "HHT_mumu_aftermetbtag2", HT,datasetName, IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[0]);
		  MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag2, "InvmumuMassPair_aftermetbtag2" ,InvDilMass,datasetName   , IsSignal, weightb[0]*Dweight[ITypeMC]*weight_SFtotal_2btag[0]);
		}
		
	      } // btag cuts
	      
	    }//met.p4.Et()>X (X==30 for ee/mumu; X==20/0 for emu)
	    
	  }//theselJets.size()>=2
	  
	  }// (DiLepton.M()<76 ||  DiLepton.M() >106) && DiLepton.M()>12
	
      } //CandType (Dilepton pair+VxP && DiLepton.M()>12)
      
    }//event->TriggerPassed()	 
    if (debugcc<maxdebugcc) {
      cout << endl ;
    }
    
  } // end loop over IChannel
  if (debugcc<maxdebugcc) {
    debugcc++;
  }
  
  
    //if (IReweight ) delete   LumiWeights ;
  
  
  
  return kTRUE;
}

//_____________________________________________________________________________
void ProofSelectorMyCutFlow::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  
  if(fProofFile) fProofFile->Print();
  if (fFile) {
    Bool_t cleanup = kFALSE;
    TDirectory *savedir = gDirectory;
    fFile->cd();
    
    
    if(producePLRPlots) plotsFurnisher.WriteHistos();
    
    
    cout << "write histo " << endl;
    MyhistoManager.WriteMyHisto(HHT_ee_aftermetbtag1,"all");
    MyhistoManager.WriteMyHisto(HHT_emu_aftermetbtag1,"all");
    MyhistoManager.WriteMyHisto(HHT_mumu_aftermetbtag1,"all");
    
    MyhistoManager.WriteMyHisto(HHT_ee_aftermetbtag2,"all");
    MyhistoManager.WriteMyHisto(HHT_emu_aftermetbtag2,"all");
    MyhistoManager.WriteMyHisto(HHT_mumu_aftermetbtag2,"all");
    
    
    //  if ( ChannelName == "emu" ){
    MyhistoManager.WriteMyHisto(HInvM_ll_pair_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_pair_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_emu_pair,"all");
    MyhistoManager.WriteMyHisto(HInvM_emu_pairzoom,"all");
    MyhistoManager.WriteMyHisto(HInvM_emu_pair_afterbtag1,"all");
    MyhistoManager.WriteMyHisto(HInvM_emu_pair_afterbtag2,"all");
    MyhistoManager.WriteMyHisto(HInvM_emu_pair_aftermetbtag1,"all");
    MyhistoManager.WriteMyHisto(HInvM_emu_pair_aftermetbtag2,"all");
    MyhistoManager.WriteMyHisto(HNjets_emu,"all");
    MyhistoManager.WriteMyHisto(HMet_emu,"all");
    MyhistoManager.WriteMyHisto(HNBjets_emu,"all");
    MyhistoManager.WriteMyHisto(HPtLpt1_emu,"all");
    MyhistoManager.WriteMyHisto(HPtLpt2_emu,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_emu,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_emu,"all");
    MyhistoManager.WriteMyHisto(HNVxP_emu,"all");
    MyhistoManager.WriteMyHisto(HNVxP_emu_aftertrigger,"all");
    MyhistoManager.WriteMyHisto(HInvM_emu_pair_Zemu,"all");
    MyhistoManager.WriteMyHisto(HMet_emu_afterbtag1,"all");
    MyhistoManager.WriteMyHisto(HMet_emu_afterbtag2,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_emu_pair_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNjets_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HMet_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNBjets_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtLpt1_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtLpt2_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNVxP_emu_aftermetcut,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_emu_pair_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HNjets_emu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HMet_emu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HNBjets_emu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtLpt1_emu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtLpt2_emu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_emu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_emu_afterjetcut,"all");
    
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_emu,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_emu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_emu_afterjetcut,"all");
    //  }
    
    //  if ( ChannelName == "mumu" ){
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair,"all");
    MyhistoManager.WriteMyHisto(HInvM_mumu_pairzoom,"all");
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterbtag1,"all");
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterbtag2,"all");
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair_aftermetbtag1,"all");
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair_aftermetbtag2,"all");
    MyhistoManager.WriteMyHisto(HNjets_mumu,"all");
    MyhistoManager.WriteMyHisto(HMet_mumu,"all");
    MyhistoManager.WriteMyHisto(HNBjets_mumu,"all");
    MyhistoManager.WriteMyHisto(HPtMu1_mumu,"all");
    MyhistoManager.WriteMyHisto(HPtMu2_mumu,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_mumu,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_mumu,"all");
    MyhistoManager.WriteMyHisto(HNVxP_mumu,"all");
    MyhistoManager.WriteMyHisto(HNVxP_mumu_aftertrigger,"all");
    MyhistoManager.WriteMyHisto(HMet_mumu_afterbtag1,"all");
    MyhistoManager.WriteMyHisto(HMet_mumu_afterbtag2,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNjets_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HMet_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNBjets_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtMu1_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtMu2_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNVxP_mumu_aftermetcut,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HNjets_mumu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HMet_mumu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HNBjets_mumu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtMu1_mumu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtMu2_mumu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_mumu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_mumu_afterjetcut,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterveto,"all");
    MyhistoManager.WriteMyHisto(HNjets_mumu_afterveto,"all");
    MyhistoManager.WriteMyHisto(HMet_mumu_afterveto,"all");
    MyhistoManager.WriteMyHisto(HNBjets_mumu_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtMu1_mumu_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtMu2_mumu_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_mumu_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_mumu_afterveto,"all");
    
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu_afterveto,"all");
    //  }
    
    //  if ( ChannelName == "ee" ){
    MyhistoManager.WriteMyHisto(HInvM_ee_pair,"all");
    MyhistoManager.WriteMyHisto(HInvM_ee_pairzoom,"all");
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterbtag1,"all");
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterbtag2,"all");
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_aftermetbtag1,"all");
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_aftermetbtag2,"all");
    MyhistoManager.WriteMyHisto(HNjets_ee,"all");
    MyhistoManager.WriteMyHisto(HMet_ee,"all");
    MyhistoManager.WriteMyHisto(HNBjets_ee,"all");
    MyhistoManager.WriteMyHisto(HPtEl1_ee,"all");
    MyhistoManager.WriteMyHisto(HPtEl2_ee,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_ee,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_ee,"all");
    MyhistoManager.WriteMyHisto(HNVxP_ee,"all");
    MyhistoManager.WriteMyHisto(HNVxP_ee_aftertrigger,"all");
    MyhistoManager.WriteMyHisto(HMet_ee_afterbtag1,"all");
    MyhistoManager.WriteMyHisto(HMet_ee_afterbtag2,"all");
    MyhistoManager.WriteMyHisto(pileup,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNjets_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HMet_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNBjets_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtEl1_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtEl2_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HNVxP_ee_aftermetcut,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HNjets_ee_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HMet_ee_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HNBjets_ee_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtEl1_ee_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtEl2_ee_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_ee_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_ee_afterjetcut,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterveto,"all");
    MyhistoManager.WriteMyHisto(HNjets_ee_afterveto,"all");
    MyhistoManager.WriteMyHisto(HMet_ee_afterveto,"all");
    MyhistoManager.WriteMyHisto(HNBjets_ee_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtEl1_ee_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtEl2_ee_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtJet1_ee_afterveto,"all");
    MyhistoManager.WriteMyHisto(HPtJet2_ee_afterveto,"all");
    
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee_aftermetcut,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee_afterjetcut,"all");
    MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee_afterveto,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_EB,"all");
    MyhistoManager.WriteMyHisto(HInvM_ee_pair_EE,"all");
    
    MyhistoManager.WriteMyHisto2D(HInvM_mumu_pair_vs_NJet_METinf10,"all");
    MyhistoManager.WriteMyHisto2D(HInvM_mumu_pair_vs_NBJet_METinf10,"all");
    MyhistoManager.WriteMyHisto2D(HInvM_mumu_pair_vs_MET_NJet0,"all");
    MyhistoManager.WriteMyHisto3D(HInvM_mumu_pair_vs_MET_NJet,"all");
    
    MyhistoManager.WriteMyHisto2D(HInvM_ee_pair_vs_NJet_METinf10,"all");
    MyhistoManager.WriteMyHisto2D(HInvM_ee_pair_vs_NBJet_METinf10,"all");
    MyhistoManager.WriteMyHisto2D(HInvM_ee_pair_vs_MET_NJet0,"all");
    MyhistoManager.WriteMyHisto3D(HInvM_ee_pair_vs_MET_NJet,"all");
    
    MyhistoManager.WriteMyHisto2D(HInvM_ee_pair_vs_PU,"all");
    MyhistoManager.WriteMyHisto2D(HInvM_mumu_pair_vs_PU,"all");
  
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM0Btag_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM0Btag_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM0Btag_emu,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM1Btag_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM1Btag_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM1Btag_emu,"all");
  
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM2Btag_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM2Btag_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvM2Btag_emu,"all");
  
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup0Btag_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup0Btag_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup0Btag_emu,"all");
    
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup1Btag_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup1Btag_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup1Btag_emu,"all");
  
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup2Btag_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup2Btag_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMSup2Btag_emu,"all");
  
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMDilepton_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMDilepton_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMDilepton_mumu,"all");

    
    
    
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_ee,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_mumu,"all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMNJet_AfterMET_emu,"all");

    
    
    
    MyhistoManager.WriteMyHisto(LeptonIso_e,  "all");
    MyhistoManager.WriteMyHisto(LeptonIso_mu,  "all");
    
    MyhistoManager.WriteMyHisto(CutFlow_mumu,  "all");
    MyhistoManager.WriteMyHisto(CutFlow_ee,    "all");
    MyhistoManager.WriteMyHisto(CutFlow_emu,   "all");
    MyhistoManager.WriteMyHisto(ErrCutFlow_mumu,  "all");
    MyhistoManager.WriteMyHisto(ErrCutFlow_ee,    "all");
    MyhistoManager.WriteMyHisto(ErrCutFlow_emu,   "all"); 
    
    
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_LeptonMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_DiLeptonMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_ClosestDiLeptonJets,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_DiLeptonJets,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_JetsMET,	"all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_ClosestJetsMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_PtJetsInMassP,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_ee_PtJetsOutMassP,   "all");
  
  
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_LeptonMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_DiLeptonMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_ClosestDiLeptonJets,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_DiLeptonJets,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_JetsMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_ClosestJetsMET,	"all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_PtJetsInMassP,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_emu_PtJetsOutMassP,	"all");
  
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_LeptonMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_DiLeptonMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_DiLeptonJets,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_ClosestDiLeptonJets,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_JetsMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_ClosestJetsMET,   "all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_PtJetsInMassP,	"all");
    MyhistoManager.WriteMyHisto(HDeltaPhi_mumu_PtJetsOutMassP,   "all");
  
    MyhistoManager.WriteMyHisto2D(HDileptPt_vs_met_mumu,   "all");
    MyhistoManager.WriteMyHisto2D(HDileptPt_vs_met_emu,   "all");
    MyhistoManager.WriteMyHisto2D(HDileptPt_vs_met_ee,   "all");
   
    MyhistoManager.WriteMyHisto(HDileptPt_mumu,   "all");
    MyhistoManager.WriteMyHisto(HDileptPt_ee,   "all");
    MyhistoManager.WriteMyHisto(HDileptPt_emu,   "all");
    
    SFtrigger->Write();
    SFDY->Write();
    SFWjets->Write(); 
    SFQCD->Write();   
    SFLeptons->Write();
    
    
   
    fHist->Write();
    fFile->Write();
    cout << "end write histo " << endl;
    
    MyhistoManager.WriteMyHisto(hemudata_emu,   "all");
    MyhistoManager.WriteMyHisto(hlistemu_ee,   "all");
    MyhistoManager.WriteMyHisto(hlistSR_ee,   "all");
    MyhistoManager.WriteMyHisto(hlistCR_ee,   "all");
    MyhistoManager.WriteMyHisto(hlistContCR_ee,   "all");
    MyhistoManager.WriteMyHisto(hlistemu_mumu,   "all");
    MyhistoManager.WriteMyHisto(hlistSR_mumu,   "all");
    MyhistoManager.WriteMyHisto(hlistCR_mumu,   "all");
    MyhistoManager.WriteMyHisto(hlistContCR_mumu,   "all");
    MyhistoManager.WriteMyHisto(NjetsCR_inMZ_mumu,   "all");
    MyhistoManager.WriteMyHisto(NjetsSR_inMZ_mumu,   "all");
    MyhistoManager.WriteMyHisto(NjetsCR_outMZ_mumu,   "all");
    MyhistoManager.WriteMyHisto(NjetsSR_outMZ_mumu,   "all");
    MyhistoManager.WriteMyHisto(DYMassCR_mumu,   "all");
    MyhistoManager.WriteMyHisto(DYMassSR_mumu,   "all");
    MyhistoManager.WriteMyHisto(NjetsCR_inMZ_ee,   "all");
    MyhistoManager.WriteMyHisto(NjetsSR_inMZ_ee,   "all");
    MyhistoManager.WriteMyHisto(NjetsCR_outMZ_ee,   "all");
    MyhistoManager.WriteMyHisto(NjetsSR_outMZ_ee,   "all");
    MyhistoManager.WriteMyHisto(DYMassCR_ee,   "all");
    MyhistoManager.WriteMyHisto(DYMassSR_ee,   "all");
    
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMNJet_mumu,   "all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMNJet_ee  ,   "all");
    MyhistoManager.WriteMyHisto(HInvM_ll_afterSelButInvMNJet_emu ,   "all");
    
    MyhistoManager.WriteMyHisto2D(HInvM_mumu_pair_vs_MET_NBJet0,"all");
    MyhistoManager.WriteMyHisto2D(HInvM_ee_pair_vs_MET_NBJet0,"all");

    
    MyhistoManager.WriteMyHisto(HNjets_ee_aftermetcutbutjetcut,    "all");  
    MyhistoManager.WriteMyHisto(HNjets_mumu_aftermetcutbutjetcut,  "all");
    MyhistoManager.WriteMyHisto(HNjets_emu_aftermetcutbutjetcut,   "all");
    
    
    
    
    
    
    //delete fProofFile;
    //The following line is mandatory to copy everythin in a common RootFile
    fOutput->Add(fProofFile);
    
    
    MyHistos_mumu.clear();
    MyHistos1_mumu.clear();
    HInvM_ll_pair_mumu.clear();
    HInvM_ll_pair_mumu_sup15.clear();
    HInvM_ll_pair_mumu_inf15.clear();
    MyHistos_ee.clear();
    MyHistos1_ee.clear();
    HInvM_ll_pair_ee.clear();
    HInvM_ll_pair_ee_sup15.clear();
    HInvM_ll_pair_ee_inf15.clear();
    HInvM_emu_pair.clear();
    HInvM_emu_pairzoom.clear();
    HInvM_emu_pair_afterbtag1.clear();
    HInvM_emu_pair_afterbtag2.clear();
    HInvM_emu_pair_aftermetbtag1.clear();
    HInvM_emu_pair_aftermetbtag2.clear();
    HNjets_emu.clear();
    HMet_emu.clear();
    HNBjets_emu.clear();
    HPtJet1_emu.clear();
    HPtJet2_emu.clear();
    HPtLpt1_emu.clear();
    HPtLpt2_emu.clear();
    HNVxP_emu.clear();
    HNVxP_emu_aftertrigger.clear();
    HInvM_emu_pair_Zemu.clear();
    
    HInvM_emu_pair_aftermetcut.clear();
    HNjets_emu_aftermetcut.clear();
    HNBjets_emu_aftermetcut.clear();
    HMet_emu_aftermetcut.clear();
    HPtJet1_emu_aftermetcut.clear();
    HPtJet2_emu_aftermetcut.clear();
    HPtLpt1_emu_aftermetcut.clear();
    HPtLpt2_emu_aftermetcut.clear();
    HNVxP_emu_aftermetcut.clear(); 
    
    HInvM_emu_pair_afterjetcut.clear();
    HNjets_emu_afterjetcut.clear();
    HNBjets_emu_afterjetcut.clear();
    HMet_emu_afterjetcut.clear();
    HPtJet1_emu_afterjetcut.clear();
    HPtJet2_emu_afterjetcut.clear();
    HPtLpt1_emu_afterjetcut.clear();
    HPtLpt2_emu_afterjetcut.clear();
    
    HDeltaPhiLMet_emu.clear();
    HDeltaPhiLMet_emu_aftermetcut.clear();
    HDeltaPhiLMet_emu_afterjetcut.clear();
    
    HHT_emu.clear();
    
    
    
    HInvM_mumu_pair.clear();
    HInvM_mumu_pairzoom.clear();
    HInvM_mumu_pair_afterbtag1.clear();
    HInvM_mumu_pair_afterbtag2.clear();
    HInvM_mumu_pair_aftermetbtag1.clear();
    HInvM_mumu_pair_aftermetbtag2.clear();
    HNjets_mumu.clear();
    HMet_mumu.clear();
    HNBjets_mumu.clear();
    HPtJet1_mumu.clear();
    HPtJet2_mumu.clear();
    HPtMu1_mumu.clear();
    HPtMu2_mumu.clear();
    HNVxP_mumu.clear();
    HNVxP_mumu_aftertrigger.clear();
    
    HInvM_mumu_pair_aftermetcut.clear();
    HNjets_mumu_aftermetcut.clear();
    HMet_mumu_aftermetcut.clear();
    HNBjets_mumu_aftermetcut.clear();
    HPtJet1_mumu_aftermetcut.clear();
    HPtJet2_mumu_aftermetcut.clear();
    HPtMu1_mumu_aftermetcut.clear();
    HPtMu2_mumu_aftermetcut.clear();
    HNVxP_mumu_aftermetcut.clear();
    
    HInvM_mumu_pair_afterjetcut.clear();
    HNjets_mumu_afterjetcut.clear();
    HMet_mumu_afterjetcut.clear();
    HNBjets_mumu_afterjetcut.clear();
    HPtJet1_mumu_afterjetcut.clear();
    HPtJet2_mumu_afterjetcut.clear();
    HPtMu1_mumu_afterjetcut.clear();
    HPtMu2_mumu_afterjetcut.clear();
    
    HInvM_mumu_pair_afterveto.clear();
    HNjets_mumu_afterveto.clear();
    HMet_mumu_afterveto.clear();
    HNBjets_mumu_afterveto.clear();
    HPtJet1_mumu_afterveto.clear();
    HPtJet2_mumu_afterveto.clear();
    HPtMu1_mumu_afterveto.clear();
    HPtMu2_mumu_afterveto.clear();
    
    HHT_mumu.clear();
    
    HDeltaPhiLMet_mumu.clear();
    HDeltaPhiLMet_mumu_aftermetcut.clear();
    HDeltaPhiLMet_mumu_afterjetcut.clear();
    HDeltaPhiLMet_mumu_afterveto.clear();
    
    HInvM_ee_pair.clear();
    HInvM_ee_pairzoom.clear();
    HInvM_ee_pair_afterbtag1.clear();
    HInvM_ee_pair_afterbtag2.clear();
    HInvM_ee_pair_aftermetbtag1.clear();
    HInvM_ee_pair_aftermetbtag2.clear();
    HNjets_ee.clear();
    HMet_ee.clear();
    HNBjets_ee.clear();
    HPtJet1_ee.clear();
    HPtJet2_ee.clear();
    HPtEl1_ee.clear();
    HPtEl2_ee.clear();
    HNVxP_ee.clear();
    HNVxP_ee_aftertrigger.clear();
    
    HInvM_ee_pair_aftermetcut.clear();
    HNjets_ee_aftermetcut.clear();
    HMet_ee_aftermetcut.clear();
    HNBjets_ee_aftermetcut.clear();
    HPtJet1_ee_aftermetcut.clear();
    HPtJet2_ee_aftermetcut.clear();
    HPtEl1_ee_aftermetcut.clear();
    HPtEl2_ee_aftermetcut.clear();
    HNVxP_ee_aftermetcut.clear();

    HInvM_ee_pair_afterjetcut.clear();
    HNjets_ee_afterjetcut.clear();
    HMet_ee_afterjetcut.clear();
    HNBjets_ee_afterjetcut.clear();
    HPtJet1_ee_afterjetcut.clear();
    HPtJet2_ee_afterjetcut.clear();
    HPtEl1_ee_afterjetcut.clear();
    HPtEl2_ee_afterjetcut.clear();
    
    HInvM_ee_pair_afterveto.clear();
    HNjets_ee_afterveto.clear();
    HMet_ee_afterveto.clear();
    HNBjets_ee_afterveto.clear();
    HPtJet1_ee_afterveto.clear();
    HPtJet2_ee_afterveto.clear();
    HPtEl1_ee_afterveto.clear();
    HPtEl2_ee_afterveto.clear();
    
    HDeltaPhiLMet_ee.clear();
    HDeltaPhiLMet_ee_aftermetcut.clear();
    HDeltaPhiLMet_ee_afterjetcut.clear();
    HDeltaPhiLMet_ee_afterveto.clear();
    
    HMet_emu_afterbtag1.clear();
    HMet_mumu_afterbtag1.clear();
    HMet_ee_afterbtag1.clear();
    HMet_emu_afterbtag2.clear();
    HMet_mumu_afterbtag2.clear();
    HMet_ee_afterbtag2.clear();
    
    HHT_ee_aftermetbtag1.clear();
    HHT_emu_aftermetbtag1.clear();
    HHT_mumu_aftermetbtag1.clear();
    
    HHT_ee_aftermetbtag2.clear();
    HHT_emu_aftermetbtag2.clear();
    HHT_mumu_aftermetbtag2.clear();
    
    hemudata_emu.clear();
    hlistemu_ee.clear();
    hlistSR_ee.clear();
    hlistCR_ee.clear();
    hlistContCR_ee.clear();
    hlistemu_mumu.clear();
    hlistSR_mumu.clear();
    hlistCR_mumu.clear();
    hlistContCR_mumu.clear();
    NjetsCR_inMZ_mumu.clear();
    NjetsSR_inMZ_mumu.clear();
    NjetsCR_outMZ_mumu.clear();
    NjetsSR_outMZ_mumu.clear();
    DYMassCR_mumu.clear();
    DYMassSR_mumu.clear();
    NjetsCR_inMZ_ee.clear();
    NjetsSR_inMZ_ee.clear();
    NjetsCR_outMZ_ee.clear();
    NjetsSR_outMZ_ee.clear();
    DYMassCR_ee.clear();
    DYMassSR_ee.clear();
    
    pileup.clear();
    datasets.clear();
    
    cout << "end write histo Denis " << endl;
     
    sumSFlept_ee    = sumSFlept_ee/nEvents_ee;
    sumSFlept_mumu  = sumSFlept_mumu/nEvents_mumu;
    sumSFlept_emu   = sumSFlept_emu/nEvents_emu;
    
    cout << "sumSFlept_ee   " << 1./sumSFlept_ee   << endl;
    cout << "sumSFlept_mumu " << 1./sumSFlept_mumu << endl;
    cout << "sumSFlept_emu  " << 1./sumSFlept_emu  << endl;
    
    
  
    //delete file1  ;
    //delete hPUMC ;  
    //delete file2 ; 
    delete anaEL;
    delete LumiWeights;
  }
}

//_____________________________________________________________________________
void ProofSelectorMyCutFlow::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
  //Possibility to retrieve information from the merged file and perform some calculation or plotting tasks
  delete event ;
  //
  // Create canvas
  //
  //TList* list = fOutput->GetOutputList() ;
  /*
    TIter next_object((TList*) fOutput);
    TObject* obj ;
    cout << "-- Retrieved objects:" << endl ;
    while ((obj = next_object())) { TString objname = obj->GetName() ; cout << " " << objname << endl ; }
    
    if ((fi = dynamic_cast<TFile *>(fOutput->FindObject("blabla.root")))) {
    cout<<"Warning"<<endl;
    fi->Write("toto.root");
    cout<<"Warning"<<endl;
    }
    // Final update
    c1->cd();
    c1->Update();
  */
}
