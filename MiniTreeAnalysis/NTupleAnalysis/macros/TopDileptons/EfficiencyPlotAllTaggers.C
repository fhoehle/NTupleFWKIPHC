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

#include "../../Tools/interface/PUWeighting.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

using namespace TopTree;
using namespace std;

int main (int argc, char* argv[])
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
  float LumiError = 0;
  string PUWeightFileName;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;

  bool IReweight             = true;
  bool IReweight_Spring11    = false;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  if(argc!=3){
         cerr<<"usage: ./EfficiencyPlot file.xml file.root"<<endl;
         //return(0);
  }
  string xmlFileName;
  if(argc!=3) xmlFileName = string ("../../config/EfficiencyPlot.xml");
  else xmlFileName = argv[1];

  cout << " xmlFileName " << xmlFileName << endl;
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
//  on s'en fout non???? -->
//  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!) 
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  //Load for PU:
  sel.GeneratePUWeight(PUWeightFileName);
  TopTree::NTEvent * event = 0;



// idee : creer un vecteur d'histo pour chaque couple (algo, discri_val) a etudier. 
// et a ala toute fin on stocke ces histo par cas dans un directory particulier d'un fichier root.
// ca doit pouvoir marcher a condition de nommer correctement les histo du vecteur
// quitte a leur rechanger de noms au moment du stockage



  const int number_of_cases=18;
  //Correspondantces:
 //algo 0 = TCHEL
 //algo 1 = SSVHE
 //algo 2 = TCHEP
 //algo 3 = SSVHP
 //algo 4 = JP
 //algo 5 = JPB
 //algo 6 = CVS
 

                                     //TCHEL,M,T,    SSVHEM, T, TCHPL,M, T    SSVHPT JPL   M     T    JPBL  M    T    CVSL  M      T
  int algoname[number_of_cases] =     {0  ,0   ,0   ,1   ,1   , 2   ,2   ,2   ,3  ,   4    ,4    ,4    ,5   ,5   ,5   ,6    ,6     ,6    }; 
  float discri_val[number_of_cases] = {1.7, 3.3,10.2,1.74,3.05,1.19 ,1.93,3.41,2.00,  0.275,0.545,0.790,1.33,2.55,3.74,0.244,0.679 ,0.898};   

  TH2F* h_t_bq[number_of_cases];
  TH2F* h_t_bq_wo[number_of_cases];
  TH2F* h_b_bq[number_of_cases];
  TH2F* h_eff_bq[number_of_cases];
  TH2F* h_err_bq[number_of_cases];
  TH2F* h_t_cq[number_of_cases];
  TH2F* h_t_cq_wo[number_of_cases];
  TH2F* h_b_cq[number_of_cases];
  TH2F* h_eff_cq[number_of_cases];
  TH2F* h_err_cq[number_of_cases];
  TH2F* h_t_lq[number_of_cases];
  TH2F* h_t_lq_wo[number_of_cases];
  TH2F* h_b_lq[number_of_cases];
  TH2F* h_eff_lq[number_of_cases];
  TH2F* h_err_lq[number_of_cases];

  double PTvec[16] = {20.,30.,35.,40.,45.,50.,55.,60.,65.,70.,75.,80.,90.,100.,120.,200.};

  for (int i_case=0; i_case<number_of_cases; i_case++){

      char name_h_t_bq[50];
      sprintf(name_h_t_bq,"h_t_bq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_t_bq_wo[50];
      sprintf(name_h_t_bq_wo,"h_t_bq_wo_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_b_bq[50];
      sprintf(name_h_b_bq,"h_b_bq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_eff_bq[50];
      sprintf(name_h_eff_bq,"h_eff_bq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_err_bq[50];
      sprintf(name_h_err_bq,"h_err_bq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);

      h_t_bq[i_case]   =  new TH2F(name_h_t_bq,"Eta vs PT"       ,15,PTvec,6,0.,2.4);
      h_t_bq_wo[i_case]=  new TH2F(name_h_t_bq_wo,"Eta vs PT"    ,15,PTvec,6,0.,2.4); 
      h_b_bq[i_case]   =  new TH2F(name_h_b_bq,"Eta vs PT"       ,15,PTvec,6,0.,2.4);
      h_eff_bq[i_case] =  new TH2F(name_h_eff_bq,"Eta vs PT"     ,15,PTvec,6,0.,2.4);
      h_err_bq[i_case] =  new TH2F(name_h_err_bq,"Eta vs PT"     ,15,PTvec,6,0.,2.4);

      char name_h_t_lq[50];
      sprintf(name_h_t_lq,"h_t_lq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_t_lq_wo[50];
      sprintf(name_h_t_lq_wo,"h_t_lq_wo_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_b_lq[50];
      sprintf(name_h_b_lq,"h_b_lq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_eff_lq[50];
      sprintf(name_h_eff_lq,"h_eff_lq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_err_lq[50];
      sprintf(name_h_err_lq,"h_err_lq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);

      h_t_lq[i_case]   =  new TH2F(name_h_t_lq,"Eta vs PT"       ,15,PTvec,6,0.,2.4);
      h_t_lq_wo[i_case]=  new TH2F(name_h_t_lq_wo,"Eta vs PT"    ,15,PTvec,6,0.,2.4); 
      h_b_lq[i_case]   =  new TH2F(name_h_b_lq,"Eta vs PT"       ,15,PTvec,6,0.,2.4);
      h_eff_lq[i_case] =  new TH2F(name_h_eff_lq,"Eta vs PT"     ,15,PTvec,6,0.,2.4);
      h_err_lq[i_case] =  new TH2F(name_h_err_lq,"Eta vs PT"     ,15,PTvec,6,0.,2.4);

      char name_h_t_cq[50];
      sprintf(name_h_t_cq,"h_t_cq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_t_cq_wo[50];
      sprintf(name_h_t_cq_wo,"h_t_cq_wo_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_b_cq[50];
      sprintf(name_h_b_cq,"h_b_cq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_eff_cq[50];
      sprintf(name_h_eff_cq,"h_eff_cq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
      char name_h_err_cq[50];
      sprintf(name_h_err_cq,"h_err_cq_algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);

      h_t_cq[i_case]   =  new TH2F(name_h_t_cq,"Eta vs PT"       ,15,PTvec,6,0.,2.4);
      h_t_cq_wo[i_case]=  new TH2F(name_h_t_cq_wo,"Eta vs PT"    ,15,PTvec,6,0.,2.4); 
      h_b_cq[i_case]   =  new TH2F(name_h_b_cq,"Eta vs PT"       ,15,PTvec,6,0.,2.4);
      h_eff_cq[i_case] =  new TH2F(name_h_eff_cq,"Eta vs PT"     ,15,PTvec,6,0.,2.4);
      h_err_cq[i_case] =  new TH2F(name_h_err_cq,"Eta vs PT"     ,15,PTvec,6,0.,2.4);

  }


  PUWeighting  thePUReweighter;
  if (IReweight ) {
    TFile* file1  = new TFile(PUWeightFileName.c_str(),"READ");

    TH1D *  hPUData   =0;
    hPUData         = (TH1D*)file1->Get("pileup");
    TH1F *  hPUMC   = new TH1F("pileup_MC", "pileup_MC", hPUData->GetXaxis()->GetNbins(), hPUData->GetXaxis()->GetXmin(), hPUData->GetXaxis()->GetXmax() );
    //if non spring11 MC
    if(!IReweight_Spring11){
      TFile* file2  = new TFile( "../data/CrossSection_pileup.root" ,"READ");
      hPUMC           = (TH1F*)file2->Get("pileup_TTbarSig");
      // histo in data, histo in Mc, use out-of-time pu in the reweighting
      //cout << "get MC histo  " << endl;
      thePUReweighter.setPUHisto( hPUData, hPUMC);
      //cout << "set MC histo in thePUReweighter " << endl;
      thePUReweighter.setUseOutOfTimePU(false); // set to true to use out-of-time PU
    }
    else{
     thePUReweighter.setPUHisto( hPUData);
     thePUReweighter.setUseOutOfTimePU(false);
    }
  }




  for (unsigned int d = 0; d < datasets.size (); d++) {


   datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    
    float weightITypeMC_save=1.; // we don't take into account the x-sec here!

    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      
      
      if(ievt%10000==0) cout << "number of processed events " << ievt << endl;
      
      datasets[d].eventTree ()->GetEntry (ievt);
      sel.LoadEvent(event);
      float weightITypeMC=1.;

           if(IReweight ){
                if(thePUReweighter.getUseOutOfTimePU()){
                 weightITypeMC = weightITypeMC_save*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);
                 }
                else{
                  if( IReweight_Spring11){
                     weightITypeMC = weightITypeMC_save*thePUReweighter.weight_Spring11(event->num_pileup_bc0);
                     }
                  else {
		  //change of Caro
		  weightITypeMC = weightITypeMC_save*thePUReweighter.weight_Summer11ITP(event->num_pileup_bc0);
                  }
                }
            }
            else weightITypeMC = weightITypeMC_save;




     // selection inspiree de doFullSelection()

      // pas de "tmeme"
      // pas de condition sur le trigger 

      // vertex
      vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
      int sel_vertex=0;
      if  (  selVertices.size()>0 ) sel_vertex=1;

      // Passage dilepton candidat
      vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
      vector<NTMuon>     selMuons     = sel.GetSelectedMuons();
      string CandType; // ee - emu - mumum or false    
      vector<NTElectron> candElec;
      vector<NTMuon> candMuon;
      sel.GetLeptonPair(candMuon, candElec, CandType );
      int sel_candidat=0;
      if  (CandType=="mumu"    || CandType=="ee"      || CandType=="emu" ) sel_candidat=1;
      // pas de SF_e ici!

      // Zveto
      float dimass = sel.DiLeptonMass (candMuon, candElec);
      int sel_pairZ=0;
//    La fonction DiLeptonMassCut n'est pas presente pour CMSSW 3_8_7, seulement plus tard...
//    if (DiLeptonMassCut (candMuon, candElec, CandType)) sel_pairZ=1; 

      // jets
//    vector<NTJet>      SelectedJets = sel.GetSelectedJets(candMuon, candElec);
//    comme on va aussi regarder les single leptons, il faut quand meme faire un removal
//    sur ces objets. c'est l'affaire de <0.1% pour les jets de quark b.
      vector<NTJet>      SelectedJets = sel.GetSelectedJets(selMuons, selElectrons);
      int sel_jet=0;
      if (SelectedJets.size()>=2 ) sel_jet=1;

      // met
      NTMET met                       = sel.GetMET();
      double METEMu = sel.GetMETCut().first;
      double METLL  = sel.GetMETCut().second;
      int sel_met=0;
      if (CandType=="emu"  && met.p4.Et () > METEMu) sel_met=1;
      if ((CandType=="mumu"    || CandType=="ee") && met.p4.Et () >METLL) sel_met=1;


      // cuts 
      int applycuts = 0;
//    if ( sel_candidat ==1 && sel_jet==1 ) applycuts=1; // "b" 
      if ( (selElectrons.size()>=1 || selMuons.size()>=1) && sel_jet==1 ) applycuts=1;  // "l" ou "c"
      if (applycuts==1) {

        for(unsigned int j=0;j<SelectedJets.size();j++){

             // only quark from a defined partonFlavour
             int sectectedflavour=0;
             int quarkorigin=-1;
             // LIGHT QUARKS 
             if (abs(SelectedJets[j].partonFlavour)==1) sectectedflavour=1; //d quark
             if (abs(SelectedJets[j].partonFlavour)==2) sectectedflavour=1; //u quark
             if (abs(SelectedJets[j].partonFlavour)==3) sectectedflavour=1; //s quark
             if (abs(SelectedJets[j].partonFlavour)==21) sectectedflavour=1; //gluon 
             if (sectectedflavour==1) quarkorigin=0;
             // B QUARKS
             if (abs(SelectedJets[j].partonFlavour)==5) {
                  sectectedflavour=1;
                  quarkorigin=5;
             }      
             // C QUARKS 
             else if (abs(SelectedJets[j].partonFlavour)==4) {
                  sectectedflavour=1;
                  quarkorigin=4;

             }
             // NOT RECOGNIZED --> LIGHT QUARKS, for the moment
             else if (SelectedJets[j].partonFlavour ==0) {
                  sectectedflavour=1;
                  quarkorigin=0;
             }


          float ptval=SelectedJets[j].p4.Pt();
          if (ptval>199.) ptval=199.;

          float etavalabs=SelectedJets[j].p4.Eta();
          if (etavalabs<0) etavalabs*=-1.;
          if (etavalabs>=2.4) etavalabs=2.399;
	
          // loop sur les diferents cas a etudier (algo,discri_val)
          for (int i_case=0; i_case<number_of_cases; i_case++){
          int sel_btag=0;
//          int AlgoBtag = sel.GetbtagAlgo();
//          if ( AlgoBtag==0 &&  SelectedJets[j].TCDiscri>=sel.GetbtagDiscriCut() ) sel_btag=1;
//          if ( AlgoBtag==1 &&  SelectedJets[j].SVDiscri>=sel.GetbtagDiscriCut() ) sel_btag=1;
//          if ( AlgoBtag==2 &&  SelectedJets[j].SMDiscri>=sel.GetbtagDiscriCut() ) sel_btag=1;
 //algo 0 = TCHEL
 //algo 1 = SSVHE
 //algo 2 = TCHEP
 //algo 3 = SSVHP
 //algo 4 = JP
 //algo 5 = JPB
 //algo 6 = CVS
          int AlgoBtag = algoname[i_case];
          if ( AlgoBtag==0 &&  SelectedJets[j].GetDiscri(string("trackCountingHighEffBJetTags"))>=discri_val[i_case]) sel_btag=1;
          if ( AlgoBtag==1 &&  SelectedJets[j].GetDiscri(string("simpleSecondaryVertexHighEffBJetTags"))>=discri_val[i_case]) sel_btag=1;
          if ( AlgoBtag==2 &&  SelectedJets[j].GetDiscri(string("trackCountingHighPurBJetTags"))>=discri_val[i_case]) sel_btag=1;
          if ( AlgoBtag==3 &&  SelectedJets[j].GetDiscri(string("simpleSecondaryVertexHighPurBJetTags"))>=discri_val[i_case]) sel_btag=1;
          if ( AlgoBtag==4 &&  SelectedJets[j].GetDiscri(string("jetProbabilityBJetTags"))>=discri_val[i_case]) sel_btag=1;
          if ( AlgoBtag==5 &&  SelectedJets[j].GetDiscri(string("jetBProbabilityBJetTags"))>=discri_val[i_case]) sel_btag=1;
          if ( AlgoBtag==6 &&  SelectedJets[j].GetDiscri(string("combinedSecondaryVertexBJetTags"))>=discri_val[i_case]) sel_btag=1;

	//if(  AlgoBtag==6 ) cout<<"DiscriAlgo6 "<<SelectedJets[j].GetDiscri(string("combinedSecondaryVertexBJetTags"))<<endl;
	//cout<<"AlgoBtag "<<AlgoBtag<<" "<<discri_val[i_case]<<" "<<sel_btag<<endl;
	//cout<<quarkorigin<<" "<<sel_candidat<<endl;

          if (quarkorigin==5 && sel_candidat ==1) { // b : en plus une paire dilepton!
             h_t_bq_wo[i_case]->Fill(ptval,etavalabs,weightITypeMC_save); 
             h_t_bq[i_case]->Fill(ptval,etavalabs,weightITypeMC); 
             if (sel_btag==1) {
                h_b_bq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
                h_eff_bq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
             }
          }
          else if (quarkorigin==4 ) {  // c : seulement au moins 1 lepton isole, sinon probl de stat!
             h_t_cq_wo[i_case]->Fill(ptval,etavalabs,weightITypeMC_save); 
             h_t_cq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
             if (sel_btag==1) {
                h_b_cq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
                h_eff_cq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
             }
          }
          else if (quarkorigin==0) { // l : seulement au moins 1 lepton isole, sinon probl de stat!
//          else if (quarkorigin==0 && sel_candidat ==1) { // l 
             h_t_lq_wo[i_case]->Fill(ptval,etavalabs,weightITypeMC_save); 
             h_t_lq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
             if (sel_btag==1) {
                h_b_lq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
                h_eff_lq[i_case]->Fill(ptval,etavalabs,weightITypeMC);
		//cout<<"it should be filled"<<endl;
             }
          }
          } // end loop i_case

        } // end loop jets
       } // end cut

    }  // end of loop over evts


  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

  for (int i_case=0; i_case<number_of_cases; i_case++){
  for (int i=1; i< h_t_bq[i_case]->GetNbinsX()+1; i++) {
    for (int j=1; j< h_t_bq[i_case]->GetNbinsY()+1; j++) {

       float effb=h_b_bq[i_case]->GetBinContent(i,j);
       float totb=h_t_bq[i_case]->GetBinContent(i,j);
       float totb_wo=h_t_bq_wo[i_case]->GetBinContent(i,j);
       float errb=0;
       if (totb>0 && totb_wo>0) {
         if (effb>0) {
          effb/=totb;
          errb=sqrt(totb_wo*effb*(1-effb))/totb_wo;
         }
         else {
          effb=1./totb;
          errb=sqrt(totb_wo*effb*(1-effb))/totb_wo;
          effb=0;
         }
       }
       else effb=0;
       h_eff_bq[i_case]->SetBinContent(i,j, effb);
       h_err_bq[i_case]->SetBinContent(i,j, errb);

       float effl=h_b_lq[i_case]->GetBinContent(i,j);
       float totl=h_t_lq[i_case]->GetBinContent(i,j);
       float totl_wo=h_t_lq_wo[i_case]->GetBinContent(i,j);
       float errl=0;
       if (totl>0 && totl_wo>0) {
         if (effl>0) {
          effl/=totl;
          errl=sqrt(totl_wo*effl*(1-effl))/totl_wo;
         }
         else {
          effl=1./totl;
          errl=sqrt(totl*effl*(1-effl))/totl;
          errl=sqrt(totl_wo*effl*(1-effl))/totl_wo;
          effl=0;
         }
       }
       else effl=0;
       h_eff_lq[i_case]->SetBinContent(i,j, effl);
       h_err_lq[i_case]->SetBinContent(i,j, errl);

       float effc=h_b_cq[i_case]->GetBinContent(i,j);
       float totc=h_t_cq[i_case]->GetBinContent(i,j);
       float totc_wo=h_t_cq_wo[i_case]->GetBinContent(i,j);
       float errc=0;
       if (totc>0 && totc_wo>0) {
         if (effc>0) {
          effc/=totc;
          errc=sqrt(totc_wo*effc*(1-effc))/totc_wo;
         }
         else {
          effc=1./totc;
          errc=sqrt(totc_wo*effc*(1-effc))/totc_wo;
          effc=0;
         }
       }
       else effc=0;
       h_eff_cq[i_case]->SetBinContent(i,j, effc);
       h_err_cq[i_case]->SetBinContent(i,j, errc);

    }
  }
  }

   string ofilename;
   if(argc==3){
         ofilename = argv[2];
   }
   else ofilename = string("eff_from_ttmadgraph")+string(".root");
   TFile* n  = new TFile(ofilename.c_str(),"RECREATE");

   //TFile *n = new TFile("eff_from_ttmadgraph.root","RECREATE");
   //TFile *n = new TFile(argv[2],"RECREATE");
   for (int i_case=0; i_case<number_of_cases; i_case++){
       TDirectory * dir = 0;
       char namedir[30];
       sprintf(namedir,"algo_%i_discri_%5.3f",algoname[i_case],discri_val[i_case]);
       dir = n->mkdir(namedir);
       dir->cd();
       h_t_bq[i_case]->SetName("h_t_bq");
       h_t_lq[i_case]->SetName("h_t_lq");
       h_t_cq[i_case]->SetName("h_t_cq");
       h_b_bq[i_case]->SetName("h_b_bq");
       h_b_lq[i_case]->SetName("h_b_lq");
       h_b_cq[i_case]->SetName("h_b_cq");
       h_eff_bq[i_case]->SetName("h_eff_bq");
       h_eff_lq[i_case]->SetName("h_eff_lq");
       h_eff_cq[i_case]->SetName("h_eff_cq");
       h_err_bq[i_case]->SetName("h_err_bq");
       h_err_lq[i_case]->SetName("h_err_lq");
       h_err_cq[i_case]->SetName("h_err_cq");

       h_t_bq[i_case]->Write();
       h_t_lq[i_case]->Write();
       h_t_cq[i_case]->Write();
       h_b_bq[i_case]->Write();
       h_b_lq[i_case]->Write();
       h_b_cq[i_case]->Write();
       h_eff_bq[i_case]->Write();
       h_eff_lq[i_case]->Write();
       h_eff_cq[i_case]->Write();
       h_err_bq[i_case]->Write();
       h_err_lq[i_case]->Write();
       h_err_cq[i_case]->Write();
   }
   n->Close();


  return (0);
}
