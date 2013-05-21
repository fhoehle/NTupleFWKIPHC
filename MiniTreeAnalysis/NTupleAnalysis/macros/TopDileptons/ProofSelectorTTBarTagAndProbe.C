#define ProofSelectorTTBarTagAndProbe_cxx

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

#include "ProofSelectorTTBarTagAndProbe.h"

//_____________________________________________________________________________
ProofSelectorTTBarTagAndProbe::ProofSelectorTTBarTagAndProbe()
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
   
  
  IReweight		= true;
  IDYestimateWithMetCut = true;
  IReweight_puUp	= false;
  IReweight_puDown	= false;
  hPUData   = 0;
  hPUMC     = 0;  
  
  checkElecIso = false;
  
  cout << "line 55 " << endl;
   //gSystem->Load("libNTuple.so");
   if (IReweight ) {
  
  
    if(datasetName == "DYToLL_M10-50"){
       string datafile = "/storage1/cms/jandrea/TopIPHC_2012_01_16/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/PUdata.root";
       string mcfile   = "/storage1/cms/jandrea/TopIPHC_2012_01_16/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/PU3DMC_Fall11.root";
       cout << "loading  LumiWeights" <<  endl;
    
       LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       //cout << "PUWeightFileName.c_str()  " << PUWeightFileName.c_str() << endl;
       //cout << " mcfile                   " << mcfile                   << endl;
       //LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       cout << "LumiWeights loaded" <<  endl;
     
       //if(!IReweight_puUp && !IReweight_puDown)  LumiWeights->weight_init( 1. );
       //if( IReweight_puDown                   )  LumiWeights->weight_init( 1. );
       //if( IReweight_puUp                     )  LumiWeights->weight_init( 1. );

     }
    else{
       string datafile = "/storage1/cms/jandrea/TopIPHC_2012_01_16/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/default73.5mb.root";
       string mcfile   = "/storage1/cms/jandrea/TopIPHC_2012_01_16/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/PU3DMC.root";
       cout << "loading  LumiWeights" <<  endl;
    
       LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       //cout << "PUWeightFileName.c_str()  " << PUWeightFileName.c_str() << endl;
       //cout << " mcfile                   " << mcfile                   << endl;
       //LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       cout << "LumiWeights loaded" <<  endl;
     
     
     
       //LumiWeights3D  = new reweight::Lumi3DReWeighting(mcfile, datafile, "pileup_TTbarSig", "pileup" );
       if(!IReweight_puUp && !IReweight_puDown)  LumiWeights->weight3D_init( 1. );
       if( IReweight_puDown                   )  LumiWeights->weight3D_init( 1. );
       if( IReweight_puUp                     )  LumiWeights->weight3D_init( 1. );

     }
  
  
  
  
  
   }
 
  cout << "line 80 " << endl;
}

//_____________________________________________________________________________
ProofSelectorTTBarTagAndProbe::~ProofSelectorTTBarTagAndProbe()
{
  // Destructor
  
  //SafeDelete(fHist);
}

//_____________________________________________________________________________
void ProofSelectorTTBarTagAndProbe::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses of the tree
  // will be set. It is normaly not necessary to make changes to the
  // generated code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running with PROOF.
  
  cout << "start init tree " << endl;
  //fChain point to the loading tree 
  fChain = tree;
  // Set branch addresses
  branch = (TBranch *) tree->GetBranch("NTEvent");
  event = new TopTree::NTEvent();
   branch->SetAddress(&event);
   //event is now retrieved and could be used in Process
   cout << "end init tree " << endl;
}

//_____________________________________________________________________________
void ProofSelectorTTBarTagAndProbe::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  cout << "start Begin " << endl;
  TString option = GetOption();
  cout << "end  Begin" << endl;
  
  
}

//_____________________________________________________________________________
void ProofSelectorTTBarTagAndProbe::SlaveBegin(TTree * tree)
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
  
  
  
  
  
  SF_trig_emu= 1.002; 
  
  
  
  
  //Denis' value
  SF_DY_em = 1.14;
  
  
  SF_BranchingRatio_ll = (0.108*9.)*(0.108*9.);
  SF_BranchingRatio_lj = (0.108*9.)*(0.676*1.5);
  SF_BranchingRatio_had = (0.676*1.5)*(0.676*1.5);
  
  
  
  
  SF_Wjets_ee = 1.;
  SF_Wjets_mm = 1.;
  SF_Wjets_em = 1.;
  
  SF_QCD_ee = 1.;
  SF_QCD_mm = 1.;
  SF_QCD_em = 1.;
  
  
  
  scaleElec = 1.0; // 1 to switch off
  resolElec = 0.0; // 0 to switch off
  
  
  ITypeMC     = -1;
  ICut        = -1;  
  
  
  
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
  
  float ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 50, 60, 100};
  
  MyhistoManager.CreateHisto(PtLepton_Probe,            "PtLepton_Probe",           datasetName,"LeptPt", "Entries", 12, ptRange);
  MyhistoManager.CreateHisto(EtaLepton_Probe,           "EtaLepton_Probe",          datasetName,"LeptEta","Entries", 21,-2.5,2.5);
  MyhistoManager.CreateHisto(PtLepton_Probe_passTight,  "PtLepton_Probe_passTight", datasetName,"LeptPt", "Entries", 12, ptRange);
  MyhistoManager.CreateHisto(EtaLepton_Probe_passTight, "EtaLepton_Probe_passTight",datasetName,"LeptEta","Entries", 21,-2.5,2.5);
  
  MyhistoManager.CreateHisto(nVertex, "nVertex", datasetName,  "nVertex", "Entries", 51, 0.5, 50.5);
  
  //***********************
  // initiate lumi reweighting
  //***********************
  
  //--------------------------------------//
  //   pileup reweighting  	
  //--------------------------------------//
  
  
  
  
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
Bool_t ProofSelectorTTBarTagAndProbe::Process(Long64_t entry)
{
  //---------------------------------------------------//
  // Main event loop: get entry
  //---------------------------------------------------//
  fChain->GetTree()->GetEntry(entry); 
  branch->GetEntry(entry);
  
  //cout << "295 " << endl;
  //---------------------------------------------------//
  //         Doing the analysis event by event
  //---------------------------------------------------//
  int debugcc=1000;
  int maxdebugcc=10;
  //cout<<"Entry "<<entry<<endl;
  sel.LoadEvent(event);
  //cout << "302 " << endl;
  //Collection of selected objects
  vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
  vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
  vector<NTMuon>     selMuons     = sel.GetSelectedMuons();
  vector<NTJet>      selJets	  = sel.GetSelectedJets();
  NTMET met			  = sel.GetMET(); 
  
  vector<NTElectron> selLooseIsoElectrons = sel.GetSelectedElectronsNoIso();
  vector<NTMuon>     selLooseIsoMuons     = sel.GetSelectedMuonsNoIso();
  
  
  ITypeMC     = -1;
  ICut	  = -1;  
  
  nVertex
  //cout << "318 " << endl;
  //Candidate pair of lepton
  string CandType; // ee - emu - mumum or false
  vector<NTElectron> candElec;
  vector<NTMuon> candMuon;
  
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
  
  //cout << "340 " << endl;
  if(datasetName=="Zjets" && dileptInvMass < 50) return kTRUE;
  
  if(datasetName=="DYToMuMu_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) )  return kTRUE;
  if(datasetName=="DYToEE_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) )  return kTRUE;
  if(datasetName=="DYToTauTau_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) )  return kTRUE;
  if(datasetName=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20)  return kTRUE;
  if(datasetName=="DYToEE_M-10To20"	   &&  dileptInvMass > 20)   return kTRUE;
  if(datasetName=="DYToTauTau_M-10To20" &&  dileptInvMass > 20)  return kTRUE;
  
  
  
  //cout << "352 " << endl;
  
  //cout << " datasets " << datasetName << " " << Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent() << endl;
  //Dweight[ITypeMC] = Luminosity*datasets[d].Xsection()/datasets[d].NofEvtsToRunOver();
  //cout << " getNSkimmedEvent " << datasets[d].getNSkimmedEvent() << endl;
  //cout << " NofEvtsToRunOver " << datasets[d].NofEvtsToRunOver() << endl;
  
  double weightITypeMC_save = Luminosity*dataset->Xsection()/dataset->getNSkimmedEvent();
  double weightITypeMC=0;
  //cout << "weightITypeMC_save " << weightITypeMC_save << endl;
  // on introduit ici la loop sur les channels!
  //cout << "363 " << endl;
  for (int IChannel=2; IChannel<3; IChannel++) {
    //      for (int IChannel=1; IChannel<2; IChannel++) {
    string ChannelName;
    if (IChannel==0) ChannelName= "mumu"; 
    else if (IChannel==1) ChannelName= "ee"; 
    else if (IChannel==2) ChannelName= "emu"; 
    
    //if(IChannel==0 ) cout <<  " test mumu 627" << endl;
    if (IChannel==0 && (datasetName=="DataEG" || datasetName=="DataMuEG")) continue;
    if (IChannel==1 && (datasetName=="DataMu" || datasetName=="DataMuEG")) continue;
    if (IChannel==2 && (datasetName=="DataMu" || datasetName=="DataEG"  )) continue;
    
    //if(IChannel==0 ) cout <<  " ok continue " << endl;
    if ( datasetName!="DataEG" && datasetName!="DataMu" && 
	 datasetName!="DataMuEG" && datasetName!="DataEGMu") {
      //cout << "IReweight " << IReweight << endl;
      //cout << "IReweight_Spring11" << IReweight_Spring11  << endl;
      
      
      
      
      if(IReweight ){
	int npu = event->num_pileup_bc0;
	weightITypeMC = weightITypeMC_save*LumiWeights->weight3D(event->num_pileup_bcm1 ,event->num_pileup_bc0,event->num_pileup_bcp1);
	 //weightITypeMC = weightITypeMC_save*LumiWeights->ITweight(npu);		  
      }
      else weightITypeMC = weightITypeMC_save;
      
    }
    else { // data   
      weightITypeMC = 1;
    }
    
    MyhistoManager.FillHisto(nVertex, "nVertex",  selVertices.size(),  datasetName, IsSignal,Dweight[ITypeMC]);  
    
    
    bool IsSignal = false;
    int LastStep = 0;
    bool IsData = false;
    double WeightForBranchingRatio = 1.;
    bool IsLJ = false;
    
    
    if ( datasetName=="TTbar" ) {
      
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
    
    
    
    
    //cout << "IChannel 692 "<< IChannel << "  IsSignal " << IsSignal << endl;
    //cout << " Dweight[ITypeMC] 685 " << weightITypeMC_save << endl;
    if ( datasetName=="TTbar" ) { 
      
      if (IsSignal ) { 
	
	ITypeMC = 0;  
	Dweight[ITypeMC]= weightITypeMC * WeightForBranchingRatio; 
      } 
      else     { 
	
	//cout << " Dweight[ITypeMC] 689 " <<weightITypeMC_save << "  ITypeMC " << ITypeMC << endl;
	ITypeMC = 1; 
	Dweight[ITypeMC]= weightITypeMC * WeightForBranchingRatio;
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
	      datasetName=="DYToTauTau_M-10To20"
	      ) { 
      ITypeMC = 2; IsSignal = false; Dweight[ITypeMC]= weightITypeMC;  
      /*  MOVE --> AFTER MET CUT
	  if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_DY_mm; }
	  else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_DY_em; }
	  else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_DY_ee; }
      */
      
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
      /*  MOVE --> AFTER MET CUT
	  if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_Wjets_mm; }
	  else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_Wjets_em; }
	  else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_Wjets_ee; }
      */          
      
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="SingleToptW" || datasetName=="TtW" || datasetName=="TbartW") { 
      ITypeMC = 4; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="WZ" || datasetName=="WW" || datasetName=="ZZ"  || datasetName=="VV") { 
      ITypeMC = 5; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
      fHist->Fill(0.,Dweight[ITypeMC] );
    }
    else if ( datasetName=="DYee" ) { 
      ITypeMC = 6; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      /*  MOVE --> AFTER MET CUT
	  if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_DY_mm; }
	  else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_DY_em; }
	  else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_DY_ee; }
      */
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    else if ( datasetName=="DYmumu" ) { 
      ITypeMC = 7; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
      /*  MOVE --> AFTER MET CUT
	  if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_DY_mm; }
	  else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_DY_em; }
	  else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_DY_ee; }
      */ 
      fHist->Fill(0.,Dweight[ITypeMC] );
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
      /*  MOVE --> AFTER MET CUT
	  if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_QCD_mm; }
	  else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_QCD_em; }
	  else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_QCD_ee; }
      */  
      
      
      
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    
    if ( datasetName=="DataEG" || datasetName=="DataMu" || 
	 datasetName=="DataMuEG" || datasetName=="DataEGMu") { 
      ITypeMC = 100; IsData = true;  Dweight[ITypeMC]= weightITypeMC;    
      fHist->Fill(0.,Dweight[ITypeMC] );
      
      
      //TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
    }
    
    if (debugcc<maxdebugcc) {
      //cout << debugcc << " IChannel " << IChannel << " ChannelName " << ChannelName << " ITypeMC " << ITypeMC 
      //    << " TabFlow1 " << TabFlow1[IChannel][ITypeMC][0] << endl;
      //            debugcc++; 
    }
    
    
    //cout << " Dweight[ITypeMC] 791 " << Dweight[ITypeMC] << endl;
    // Seuelemnt pour PlotsFurnisherDeMerde
    //    if ( ChannelName=="ee" ) std::cout<<"code reference before "<<Dweight[ITypeMC] <<std::endl;
    string CandType = "";
    double LeptonSF = 0.; 
    if(checkElecIso) sel.GetLeptonPair(sel.GetSelectedMuons(), sel.GetSelectedElectronsNoIso(), candMuon, candElec, CandType );
    else             sel.GetLeptonPair(sel.GetSelectedMuonsNoIso(), sel.GetSelectedElectrons(), candMuon, candElec, CandType );
    
    
    
    
    if (  sel.passTriggerSelection ( dataset, ChannelName)  && CandType == "emu"  ) {
      
      //TabFlow1[IChannel][ITypeMC][10]+=Dweight[ITypeMC];
      //TabFlow2[IChannel][ITypeMC][10]+=Dweight[ITypeMC]*Dweight[ITypeMC];
      
      //         add SF_trigger here   
      if (ChannelName=="emu"  &&  !IsData ){
	Dweight[ITypeMC]*=SF_trig_emu;
      }
      
      
      
      TLorentzVector dileptonCand;
      dileptonCand+=candMuon[0].p4+candElec[0].p4;
      double InvDilMass = dileptonCand.M();
      
      
      //      Passage dilepton candidat
      if ( (CandType=="emu"  && ChannelName=="emu")     
	   && selVertices.size()>0 
	   //&& InvDilMass> sel.GetMinValueMassCut() && dileptonCand.Pt() > 15) {
	   && InvDilMass> 25 ) {
	if (debugcc<maxdebugcc) {
	  cout << " selpair " ;
	}
	
	
	
	vector<NTJet>      theselJets = sel.GetSelectedJets(candMuon, candElec);
	int JetMul = theselJets.size();
	if ( JetMul>3 ) JetMul=4;
	float misset = met.p4.Et();
	
	int NBtaggedJets = 0;
	int AlgoBtag = sel.GetbtagAlgo();
	for(unsigned int j=0;j<theselJets.size();j++){
	  if ( AlgoBtag==0 &&  theselJets[j].TCDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	  if ( AlgoBtag==1 &&  theselJets[j].SVDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	  if ( AlgoBtag==2 &&  theselJets[j].SMDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
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
	    vector < float >weight_temp = sel.GetSFBweight().GetWeigth4BSel (sel.GetMethodb(), sel.GetSystb(),theselJets);
	    weightb[0] = weight_temp[0]; //weight of the event
	    weightb[1] = weight_temp[1];  //proba 0 jet
	    weightb[2] = weight_temp[2];  //proba 1 jet
	    weightb[3] = weight_temp[3];  //proba 2 jets
	    weightb[4] = weight_temp[4];  //proba at least 3 jets
	  }
	  
	}
	
	int NBtaggedJets3 = NBtaggedJets;
	if (NBtaggedJets3>3) NBtaggedJets3 = 3; 
	
	
	// Passage invariante mass dilepton
	//	  if (( InvDilMass<sel.GetZmassWindowCut().first || InvDilMass>sel.GetZmassWindowCut().second ) && (InvDilMass> sel.GetMinValueMassCut() ))  {
	if ( ( (ChannelName=="mumu" || ChannelName=="ee") && 
	       sel.DiLeptonMassCut(sel.GetMinValueMassCut(),sel.GetZmassWindowCut(),candMuon,candElec,ChannelName) ) || 
	     //(ChannelName=="emu" && InvDilMass> sel.GetMinValueMassCut() && dileptonCand.Pt()>15)
	     (ChannelName=="emu" && InvDilMass> 25 )
	     )  {
	  
	  
	  
	  
	  // Passage multiplicite de jets
	  if ( theselJets.size()>=2 )  {
	    //TabFlow1[IChannel][ITypeMC][4]+=Dweight[ITypeMC];
	    //TabFlow2[IChannel][ITypeMC][4]+=Dweight[ITypeMC]*Dweight[ITypeMC];
	    
	    
	    
	    // Passage met
	    if ( (ChannelName=="emu"  &&  met.p4.Et()>sel.GetMETCut().first)    )  {
	      
	      // APPLY SF_DD HERE! :
	      if (ChannelName=="emu" && CandType=="emu" ){
		if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { Dweight[ITypeMC]*=SF_DY_em; }
		else if (ITypeMC==3) { Dweight[ITypeMC]*=SF_Wjets_em; }
		else if (ITypeMC==8) { Dweight[ITypeMC]*=SF_QCD_em; }
	      }
	      
	      
	      
	      
	      
	      // Passage Btagging
	      
	      
	      if (sel.GetFlagb() == 0 || IsData) {
		if ( NBtaggedJets>=2  && ( (candMuon.size()+candElec.size() ) ==2 ) )  {
		  
		  double leptPt = 0;
		  double leptEta = 0;
		  
		  if(checkElecIso ){
		    leptPt  = candElec[0].p4.Pt();
		    leptEta = candElec[0].p4.Eta();
		  }
		  else{
		    leptPt  = candMuon[0].p4.Pt();
		    leptEta = candMuon[0].p4.Eta();
		  }
		  MyhistoManager.FillHisto(PtLepton_Probe, "PtLepton_Probe",  leptPt,  datasetName, IsSignal,Dweight[ITypeMC]);  
		  MyhistoManager.FillHisto(EtaLepton_Probe,"EtaLepton_Probe", leptEta, datasetName, IsSignal,Dweight[ITypeMC]);
		  
		  if( (checkElecIso && candElec[0].RelIso03PF() < 0.17) || (!checkElecIso && candMuon[0].RelIso03PF() < 0.20 )  ){	  
		    MyhistoManager.FillHisto(PtLepton_Probe_passTight, "PtLepton_Probe_passTight",   leptPt,  datasetName, IsSignal,Dweight[ITypeMC]);  
		    MyhistoManager.FillHisto(EtaLepton_Probe_passTight, "EtaLepton_Probe_passTight", leptEta, datasetName, IsSignal,Dweight[ITypeMC]); 
		  } 
		}
	      }
	      else if (sel.GetFlagb() == 1 && ( (candMuon.size()+candElec.size() ) ==2 )) {
		double leptPt = 0;
		double leptEta = 0;
		
		if(checkElecIso){
		  leptPt  = candElec[0].p4.Pt();
		  leptEta = candElec[0].p4.Eta();
		}
		else{
		  leptPt  = candMuon[0].p4.Pt();
		  leptEta = candMuon[0].p4.Eta();
		}
		MyhistoManager.FillHisto(PtLepton_Probe, "PtLepton_Probe",                       leptPt,  datasetName, IsSignal,weightb[0]*Dweight[ITypeMC]);  
		MyhistoManager.FillHisto(EtaLepton_Probe,"EtaLepton_Probe",                      leptEta, datasetName, IsSignal,weightb[0]*Dweight[ITypeMC]);
		
		if( (checkElecIso && candElec[0].RelIso03PF() < 0.17) || (!checkElecIso && candMuon[0].RelIso03PF() < 0.20 )  ){	  
		  MyhistoManager.FillHisto(PtLepton_Probe_passTight, "PtLepton_Probe_passTight",   leptPt,  datasetName, IsSignal,weightb[0]*Dweight[ITypeMC]);  
		  MyhistoManager.FillHisto(EtaLepton_Probe_passTight, "EtaLepton_Probe_passTight", leptEta, datasetName, IsSignal,weightb[0]*Dweight[ITypeMC]); 
		}
	      }// btag cuts
	    }//met.p4.Et()>X (X==30 for ee/mumu; X==20/0 for emu)
	    
	  }//theselJets.size()>=2
	  
	}// (DiLepton.M()<76 ||  DiLepton.M() >106) && DiLepton.M()>12
	
      } //CandType (Dilepton pair+VxP && DiLepton.M()>12)
      
    }//event->TriggerPassed()
    
  } // end loop over IChannel
  
  
  //if (IReweight ) delete   LumiWeights ;
  
  
  
  return kTRUE;
}

//_____________________________________________________________________________
void ProofSelectorTTBarTagAndProbe::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  
  if(fProofFile) fProofFile->Print();
  if (fFile) {
    Bool_t cleanup = kFALSE;
    TDirectory *savedir = gDirectory;
    fFile->cd();
    
    
    
    
    cout << "write histo " << endl;
    //MyhistoManager.WriteMyHisto(HHT_ee_aftermetbtag1,"all");
    MyhistoManager.WriteMyHisto(PtLepton_Probe,"all");
    MyhistoManager.WriteMyHisto(EtaLepton_Probe,"all");
    MyhistoManager.WriteMyHisto(PtLepton_Probe_passTight,"all");
    MyhistoManager.WriteMyHisto(EtaLepton_Probe_passTight,"all");
    
    fFile->Write();
    cout << "end write histo " << endl;
    fOutput->Add(fProofFile);
    //delete file1  ;
    //delete hPUMC ;  
    //delete file2 ; 
    delete anaEL;
    delete LumiWeights;
  }
}

//_____________________________________________________________________________
void ProofSelectorTTBarTagAndProbe::Terminate()
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
