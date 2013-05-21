#include "../interface/AMWT.h"
#include "../interface/Constants.h"
#include "../../Tools/interface/FileExists.h"

#include "TVector2.h"





//#include <LHAPDF/LHAPDF.h>
#include </opt/sbg/data/data1/cms/jandrea/LHAPDF/lhapdf-5.8.5/final/include/LHAPDF/LHAPDF.h>


AMWT::AMWT(AnalysisEnvironmentLoader & anaEL, bool isData) 
{
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  nMassPts  = 500;
  rangeLow  = 100;
  anaEL.LoadAnalysisInfo("MassAnalysis", "RangeLow", rangeLow);
  anaEL.LoadAnalysisInfo("MassAnalysis", "nMassPts", nMassPts);
  rangeHigh = rangeLow+nMassPts;

  if (isData) anaEL.LoadAnalysisInfo("MassAnalysis", "NbrJetSmearData", nbrJetSmear);
   else anaEL.LoadAnalysisInfo("MassAnalysis", "NbrJetSmearMC", nbrJetSmear);
  cout << "Number of iterations: " << nbrJetSmear<<endl;

   //Solver class
  massSolver= new MassSolver();


   //PDF initialization
// fixme
  string lhapdfFile;
  anaEL.LoadAnalysisInfo("LHAPDF", "file", lhapdfFile);
  cout << "LHAPDF file "<<lhapdfFile<<endl;
  fexists(lhapdfFile, true);
  LHAPDF::initPDFSet(lhapdfFile);

  //TRandom 3 class
  rand3 = new TRandom3();

  //Jet resolution functions
  bool doGaussian;
  string JR_Standalone_Path;
  anaEL.LoadAnalysisInfo("JetResolution", "path", JR_Standalone_Path);
  anaEL.LoadAnalysisInfo("JetResolution", "doGaussian", doGaussian);
  cout << "JetResolution path " <<JR_Standalone_Path<<endl;
  string jetAlgo;
  anaEL.LoadAnalysisInfo("JetResolution", "jetAlgo", jetAlgo);

  string ptFileName, etaFileName, phiFileName;

  if (jetAlgo == "ak5"){
    ptFileName  = JR_Standalone_Path+"Spring10_PtResolution_AK5Calo.txt";
    etaFileName = JR_Standalone_Path+"Spring10_EtaResolution_AK5Calo.txt";
    phiFileName = JR_Standalone_Path+"Spring10_PhiResolution_AK5Calo.txt";
  } else if (jetAlgo == "pf"){
    ptFileName  = JR_Standalone_Path+"Spring10_PtResolution_AK5PF.txt";
    etaFileName = JR_Standalone_Path+"Spring10_EtaResolution_AK5PF.txt";
    phiFileName = JR_Standalone_Path+"Spring10_PhiResolution_AK5PF.txt";
  } else {
    cout << "Jet algo not known: "<< jetAlgo<<endl;
    assert(false);
  }
  fexists(ptFileName, true);
  fexists(etaFileName, true);
  fexists(phiFileName, true);
  ptResol  = new JetResolution(ptFileName,doGaussian);
  etaResol = new JetResolution(etaFileName,doGaussian);
  phiResol = new JetResolution(phiFileName,doGaussian);


}

/*bool AMWT::fexists(const string filename, bool fail)
{
  ifstream ifile(filename.c_str());
  if (fail && !ifile) {
    cout << "File does not exist: " << filename << endl;
    assert(ifile);
  }
  return ifile;
}*/

AMWT::~AMWT() 
{
  delete massSolver;

}

TH1F * AMWT::sampleWeightHisto() const 
{
  return new TH1F("massWeightHistoSample","MassWeight Histo", nMassPts, rangeLow, rangeHigh);
}

TopMassVariables * AMWT::findMass(const DiLeptonSelection & sel)
{
   //Top Event class
  TopMassVariables * topMassVariables = new TopMassVariables(sel);
  if (topMassVariables->nJets() < 2) return topMassVariables;

  vector < TLorentzVector > nu1;
  vector < TLorentzVector > nu2;
  TLorentzVector lvTop1;
  TLorentzVector lvTop2;

// 	 double max_sum_weight_all = 0.;
// 	 unsigned int msw_jet_all1 = SENTINEL;
// 	 unsigned int msw_jet_all2 = SENTINEL;

  float max_weight_all = 0.;
  TH1F * h_temp_MassWeight   = new TH1F("massWeightHisto","MassWeight Histo", nMassPts, rangeLow, rangeHigh);
//   TH1F * h_temp_MassWeight_1 = new TH1F("h_temp_MassWeight_1","h_temp_MassWeight_1", nMassPts, rangeLow, rangeHigh);
//   TH1F * h_temp_MassWeight_2 = new TH1F("h_temp_MassWeight_2","h_temp_MassWeight_2", nMassPts, rangeLow, rangeHigh);

  //Shift the jets by an energy scale factor
  TVector2 const & met_sh = topMassVariables->metV();
  vector <TLorentzVector> jets_by_pt_sh;
  jets_by_pt_sh.push_back(topMassVariables->selectedJetLV(0));
  jets_by_pt_sh.push_back(topMassVariables->selectedJetLV(1));
      // cout << "1\n";

// 	 if (JES_JetMET == 1.00){
// 	 else if (fabs(JES_JetMET) > 0.5) shiftJetMET      (topMassVariables->v_jets, met, jets_by_pt_sh, met_sh, JES_JetMET);
// 	 else                             shiftJetMETvsEta (topMassVariables->v_jets, met, jets_by_pt_sh, met_sh, JES_JetMET);

      // cout << "1\n";
  vector <TF1*> vPtRes;
  vector <TF1*> vEtaRes;
  vector <TF1*> vPhiRes;

      // cout << "1\n";

  if (nbrJetSmear != 1){
    for (unsigned ijr = 0; ijr < jets_by_pt_sh.size(); ijr++){
      TF1* fPtResol  = (TF1*) ptResol ->resolutionEtaPt(jets_by_pt_sh.at(ijr).Eta(), jets_by_pt_sh.at(ijr).Pt())->Clone();
      TF1* fEtaResol = (TF1*) etaResol->resolutionEtaPt(jets_by_pt_sh.at(ijr).Eta(), jets_by_pt_sh.at(ijr).Pt())->Clone();
      TF1* fPhiResol = (TF1*) phiResol->resolutionEtaPt(jets_by_pt_sh.at(ijr).Eta(), jets_by_pt_sh.at(ijr).Pt())->Clone();

      vPtRes .push_back(fPtResol);
      vEtaRes.push_back(fEtaResol);
      vPhiRes.push_back(fPhiResol);
    }
  }

	 //Begin JetMET smearing loop---------------------------------------------------------------
  TVector2 met_sm;
  vector <TLorentzVector> jets_by_pt_sm;
  double m_top_iter, sum_weight;

  for (int iSM_JMT = 0; iSM_JMT < nbrJetSmear; iSM_JMT++){
    if (verbosity >= 3) cout << "Smearing iteration "<< iSM_JMT<<endl;

    if (nbrJetSmear == 1) dont_smear_JetMET(jets_by_pt_sh, met_sh, jets_by_pt_sm, met_sm);
    else smear_JetMET(jets_by_pt_sh, met_sh, jets_by_pt_sm, met_sm, rand3, vPtRes, vEtaRes,
			vPhiRes, topMassVariables->diLeptonLV());

    for (int i_t = 0; i_t < nMassPts; i_t++){
      m_top_iter = rangeLow + i_t;

      sum_weight = 0;
//       double h_weight = 0;
//       double sum_weight_1 = 0;

      //Clear neutrino vectors and solve
      nu1.clear();
      nu2.clear();
      massSolver->solve( met_sm , jets_by_pt_sm[0], jets_by_pt_sm[1],
		topMassVariables->posLeptonLV(), topMassVariables->negLeptonLV(), m_W, m_W, m_top_iter, m_top_iter, nu1, nu2 );

      if (verbosity >= 3) cout << "Solutions for mtop "<<m_top_iter<<" : "<<nu1.size();

      for (unsigned int ui = 0; ui < nu1.size(); ui++){
	double s_weight = get_weight(jets_by_pt_sm[0], jets_by_pt_sm[1],
		topMassVariables->posLeptonLV(), topMassVariables->negLeptonLV(), nu1[ui], nu2[ui], m_top_iter);
	if (verbosity >= 3) cout << " - " << s_weight;
	sum_weight += s_weight;
	//sum_weight_1 += s_weight;
	// if (h_weight < s_weight) h_weight = s_weight;
	if (max_weight_all < s_weight){
	  max_weight_all = s_weight;
	  lvTop1 = topMassVariables->posLeptonLV() + nu1[ui] + jets_by_pt_sm[0];
	  lvTop2 = topMassVariables->negLeptonLV() + nu2[ui] + jets_by_pt_sm[1];
	}
      }

//       if (sum_weight_1 > 0) h_temp_MassWeight_1->Fill(m_top_iter, sum_weight_1);

      //double sum_weight_2 = 0;

      //Clear neutrino vectors and solve
      nu1.clear();
      nu2.clear();
      massSolver->solve(met_sm, jets_by_pt_sm[1], jets_by_pt_sm[0],
		topMassVariables->posLeptonLV(), topMassVariables->negLeptonLV(), m_W, m_W, m_top_iter, m_top_iter, nu1, nu2 );
      if (verbosity >= 3) cout << "\nSolutions for mtop "<<m_top_iter<<" : "<<nu1.size();

      for (unsigned int ui = 0; ui < nu1.size(); ui++){
	double s_weight = get_weight(jets_by_pt_sm[1], jets_by_pt_sm[0],
		topMassVariables->posLeptonLV(), topMassVariables->negLeptonLV(), nu1[ui], nu2[ui], m_top_iter);
	if (verbosity >= 3) cout << " - " << s_weight;
	sum_weight += s_weight;
	//sum_weight_2 += s_weight;
	//if (h_weight < s_weight) h_weight = s_weight;
	if (max_weight_all < s_weight){
	  max_weight_all = s_weight;
	  lvTop1 = topMassVariables->posLeptonLV() + nu1[ui] + jets_by_pt_sm[1];
	  lvTop2 = topMassVariables->negLeptonLV() + nu2[ui] + jets_by_pt_sm[0];
	}
      }
      if (verbosity >= 3) cout <<endl;

//       if (sum_weight_2 > 0) h_temp_MassWeight_2->Fill(m_top_iter, sum_weight_2);

      if (sum_weight > 0) h_temp_MassWeight->Fill(m_top_iter, sum_weight);

    }    //End loop over mass values
  }      //End JetMet smearing loop
       

  //Check if the highest weight is not zero
  topMassVariables->setMass(h_temp_MassWeight);

  if (topMassVariables->peakWeight() > 0.) {
    topMassVariables->setTopPt(lvTop1.Pt(), lvTop2.Pt(),(lvTop1 + lvTop2).Pt());
    
  }

  for (unsigned int i = 0; i < vPtRes.size(); i++){
    delete vPtRes[i];
    delete vEtaRes[i];
    delete vPhiRes[i];
  }

  return topMassVariables;
}


TopMassVariables * AMWT::findMass(TopMassVariables & topMassVariables)
{
   //Top Event class
  //TopMassVariables * topMassVariables = new TopMassVariables(sel);
  if (topMassVariables.nJets() < 2) return &topMassVariables;

  vector < TLorentzVector > nu1;
  vector < TLorentzVector > nu2;
  
  
  vector< TLorentzVector > lvTop1;
  vector< TLorentzVector > lvTop2;
  
  vector < float > max_weight_all ;
  vector < float > topmass_scan ;
  
  //float max_weight_all = 0.;
  
  
  
  
  

// 	 double max_sum_weight_all = 0.;
// 	 unsigned int msw_jet_all1 = SENTINEL;
// 	 unsigned int msw_jet_all2 = SENTINEL;

  TH1F * h_temp_MassWeight   = new TH1F("massWeightHisto","MassWeight Histo", nMassPts, rangeLow, rangeHigh);
//   TH1F * h_temp_MassWeight_1 = new TH1F("h_temp_MassWeight_1","h_temp_MassWeight_1", nMassPts, rangeLow, rangeHigh);
//   TH1F * h_temp_MassWeight_2 = new TH1F("h_temp_MassWeight_2","h_temp_MassWeight_2", nMassPts, rangeLow, rangeHigh);

  //Shift the jets by an energy scale factor
  TVector2 const & met_sh = topMassVariables.metV();
  vector <TLorentzVector> jets_by_pt_sh;
  jets_by_pt_sh.push_back(topMassVariables.selectedJetLV(0));
  jets_by_pt_sh.push_back(topMassVariables.selectedJetLV(1));
      // cout << "1\n";

// 	 if (JES_JetMET == 1.00){
// 	 else if (fabs(JES_JetMET) > 0.5) shiftJetMET      (topMassVariables->v_jets, met, jets_by_pt_sh, met_sh, JES_JetMET);
// 	 else                             shiftJetMETvsEta (topMassVariables->v_jets, met, jets_by_pt_sh, met_sh, JES_JetMET);

      // cout << "1\n";
  vector <TF1*> vPtRes;
  vector <TF1*> vEtaRes;
  vector <TF1*> vPhiRes;

      // cout << "1\n";

  if (nbrJetSmear != 1){
    for (unsigned ijr = 0; ijr < jets_by_pt_sh.size(); ijr++){
      TF1* fPtResol  = (TF1*) ptResol ->resolutionEtaPt(jets_by_pt_sh.at(ijr).Eta(), jets_by_pt_sh.at(ijr).Pt())->Clone();
      TF1* fEtaResol = (TF1*) etaResol->resolutionEtaPt(jets_by_pt_sh.at(ijr).Eta(), jets_by_pt_sh.at(ijr).Pt())->Clone();
      TF1* fPhiResol = (TF1*) phiResol->resolutionEtaPt(jets_by_pt_sh.at(ijr).Eta(), jets_by_pt_sh.at(ijr).Pt())->Clone();

      vPtRes .push_back(fPtResol);
      vEtaRes.push_back(fEtaResol);
      vPhiRes.push_back(fPhiResol);
    }
  }
  
  
  
  
  //*****************************************************************************************
  //Begin JetMET smearing loop---------------------------------------------------------------
  //*****************************************************************************************
  TVector2 met_sm;
  vector <TLorentzVector> jets_by_pt_sm;
  double m_top_iter, sum_weight;

  for (int iSM_JMT = 0; iSM_JMT < nbrJetSmear; iSM_JMT++){
    if (verbosity >= 3) cout << "Smearing iteration "<< iSM_JMT<<endl;

    if (nbrJetSmear == 1) dont_smear_JetMET(jets_by_pt_sh, met_sh, jets_by_pt_sm, met_sm);
    else smear_JetMET(jets_by_pt_sh, met_sh, jets_by_pt_sm, met_sm, rand3, vPtRes, vEtaRes,
			vPhiRes, topMassVariables.diLeptonLV());

    for (int i_t = 0; i_t < nMassPts; i_t++){
    
      //initialize 4-vector and max weights
      TLorentzVector init_p4(0, 0, 0 ,0);
      lvTop1.push_back(init_p4);
      lvTop2.push_back(init_p4);
      
      max_weight_all.push_back(0);
  
      m_top_iter = rangeLow + i_t;
      
      topmass_scan.push_back(m_top_iter) ;

      sum_weight = 0;
//       double h_weight = 0;
//       double sum_weight_1 = 0;

      //Clear neutrino vectors and solve
      nu1.clear();
      nu2.clear();
      massSolver->solve( met_sm , jets_by_pt_sm[0], jets_by_pt_sm[1],
		topMassVariables.posLeptonLV(), topMassVariables.negLeptonLV(), m_W, m_W, m_top_iter, m_top_iter, nu1, nu2 );

      if (verbosity >= 3) cout << "Solutions for mtop "<<m_top_iter<<" : "<<nu1.size();

      for (unsigned int ui = 0; ui < nu1.size(); ui++){
	double s_weight = get_weight(jets_by_pt_sm[0], jets_by_pt_sm[1],
		topMassVariables.posLeptonLV(), topMassVariables.negLeptonLV(), nu1[ui], nu2[ui], m_top_iter);
	if (verbosity >= 3) cout << " - " << s_weight;
	sum_weight += s_weight;
	//sum_weight_1 += s_weight;
	// if (h_weight < s_weight) h_weight = s_weight;
	if (max_weight_all[i_t] < s_weight){
	  max_weight_all[i_t] = s_weight;
	  lvTop1[i_t] = topMassVariables.posLeptonLV() + nu1[ui] + jets_by_pt_sm[0];
	  lvTop2[i_t] = topMassVariables.negLeptonLV() + nu2[ui] + jets_by_pt_sm[1];
	}
      }

//       if (sum_weight_1 > 0) h_temp_MassWeight_1->Fill(m_top_iter, sum_weight_1);

      //double sum_weight_2 = 0;

      //Clear neutrino vectors and solve
      nu1.clear();
      nu2.clear();
      massSolver->solve(met_sm, jets_by_pt_sm[1], jets_by_pt_sm[0],
		topMassVariables.posLeptonLV(), topMassVariables.negLeptonLV(), m_W, m_W, m_top_iter, m_top_iter, nu1, nu2 );
      if (verbosity >= 3) cout << "\nSolutions for mtop "<<m_top_iter<<" : "<<nu1.size();

      for (unsigned int ui = 0; ui < nu1.size(); ui++){
	double s_weight = get_weight(jets_by_pt_sm[1], jets_by_pt_sm[0],
		topMassVariables.posLeptonLV(), topMassVariables.negLeptonLV(), nu1[ui], nu2[ui], m_top_iter);
	if (verbosity >= 3) cout << " - " << s_weight;
	sum_weight += s_weight;
	//sum_weight_2 += s_weight;
	//if (h_weight < s_weight) h_weight = s_weight;
	if (max_weight_all[i_t] < s_weight){
	  max_weight_all[i_t] = s_weight;
	  lvTop1[i_t] = topMassVariables.posLeptonLV() + nu1[ui] + jets_by_pt_sm[1];
	  lvTop2[i_t] = topMassVariables.negLeptonLV() + nu2[ui] + jets_by_pt_sm[0];
	}
      }
      if (verbosity >= 3) cout <<endl;

//       if (sum_weight_2 > 0) h_temp_MassWeight_2->Fill(m_top_iter, sum_weight_2);

      if (sum_weight > 0) h_temp_MassWeight->Fill(m_top_iter, sum_weight);

    }    //End loop over mass values
  }      //End JetMet smearing loop
       

  //Check if the highest weight is not zero
  topMassVariables.setMass(h_temp_MassWeight);
  
  
  double topmass_final = topMassVariables.peakMass();
  
  int it_topmass = (topmass_final - rangeLow);
  
  if (topMassVariables.peakWeight() > 0.) {
    topMassVariables.setTopPt( lvTop1[it_topmass].Pt(), lvTop2[it_topmass].Pt(),(lvTop1[it_topmass]    + lvTop2[it_topmass]).Pt());
    topMassVariables.setTop1P4(lvTop1[it_topmass].Px(), lvTop1[it_topmass].Py(), lvTop1[it_topmass].Pz(),lvTop1[it_topmass].E() ); 
    topMassVariables.setTop2P4(lvTop2[it_topmass].Px(), lvTop2[it_topmass].Py(), lvTop2[it_topmass].Pz(),lvTop2[it_topmass].E() ); 
  }
  
  
  
  
  
  
  
  for (unsigned int i = 0; i < vPtRes.size(); i++){
    delete vPtRes[i];
    delete vEtaRes[i];
    delete vPhiRes[i];
  }

  return &topMassVariables;
}

double AMWT::get_weight(const TLorentzVector & bquark1, const TLorentzVector & bquark2,
	const TLorentzVector & lep_p, const TLorentzVector & lep_m, const TLorentzVector & nu1,
	const TLorentzVector & nu2, const double top_mass) const
{

  TLorentzVector t1 = lep_p + nu1 + bquark1;
  TLorentzVector t2 = lep_m + nu2 + bquark2;

  //Get
  double prob_dalitz = 1.0;
  prob_dalitz *= get_dalitz_prob( lep_p, t1, m_bquark, m_W );
  prob_dalitz *= get_dalitz_prob( lep_m, t2, m_bquark, m_W );

  //Determine x1 and x2
  double x1 = ( t1.E() + t2.E() + t1.Pz() + t2.Pz() ) / e_com;
  double x2 = ( t1.E() + t2.E() - t1.Pz() - t2.Pz() ) / e_com;

  vector <double> f1, f2;

  f1 = LHAPDF::xfx(x1, top_mass);
  f2 = LHAPDF::xfx(x2, top_mass);

  // The order of f:
  //    -t  -b  -c  -s  -u  -d   g   d   u   s   c   b   t
  //    -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6
  //     0   1   2   3   4   5   6   7   8   9   10  11  12

  double sbar1 = f1[3], sbar2 = f2[3];
  double ubar1 = f1[4], ubar2 = f2[4];
  double dbar1 = f1[5], dbar2 = f2[5];
  double g1    = f1[6], g2    = f2[6];
  double d1    = f1[7], d2    = f2[7];
  double u1    = f1[8], u2    = f2[8];
  double s1    = f1[9], s2    = f2[9];

  //Should glue-glue be doubled? Probably not, but plot histo later
  double pdf_prob = (u1*ubar2 + u2*ubar1 +
		     d1*dbar2 + d2*dbar1 +
		     s1*sbar2 + s2*sbar1 +
		     g1*g2);

//   double tt_pt_prob = get_top_pt_prob(t1.Pt()) * get_top_pt_prob(t2.Pt());

//   double two_bjet_prob = get_2bjet_prob(bquark1, bquark2, mapJetPhi2Discr);

//   double s_weight = pdf_prob * prob_dalitz * tt_pt_prob * two_bjet_prob;
//   double s_weight = pdf_prob * prob_dalitz * two_bjet_prob;
//   double s_weight = pdf_prob * prob_dalitz * tt_pt_prob;
  return pdf_prob * prob_dalitz;
}

double AMWT::get_dalitz_prob(const TLorentzVector & lep, const TLorentzVector & top,
		double mb, double mw) const
{
    double mte = lep.Dot( top );
    double mt = top.M();
    double mt2 = mt * mt;
    double mb2 = mb * mb;
    double mw2 = mw * mw;
    double mt2_mb2 = mt2 - mb2;

    return 4. * mte * ( mt2 - mb2 - 2. * mte ) / 
	( mt2_mb2 * mt2_mb2 + mw2 * ( mt2 + mb2 ) - 2. * mw2 * mw2 );
}

double AMWT::get_2bjet_prob(const TLorentzVector & jet1, const TLorentzVector & jet2,
		            map <double,double> & mapJetPhi2Discr) const
{

  double proba1 =exp(-4*mapJetPhi2Discr[jet1.Phi()]);
  double proba2 =exp(-4*mapJetPhi2Discr[jet2.Phi()]);

  return (1-proba1)*(1-proba2);
}


double AMWT::get_top_pt_prob(const double pt) const
{

  double prob_array[120] = {
    0.00200245,	 0.0060801,	0.00999056,	0.013597,	0.0170136,	0.0203661,	0.0231052,	0.0259027,	0.0279369,	0.0299741,
    0.0313266,	 0.0328515,	0.0333733,	0.0339433,	0.0341794,	0.0340638,	0.0339737,	0.0336051,	0.0332196,	0.0317563,
    0.0308568,	 0.0299114,	0.028731,	0.0275211,	0.0260246,	0.0246938,	0.0234093,	0.022064,	0.0207067,	0.0193499,
    0.0179213,	 0.0169745,	0.0156669,	0.0145741,	0.013649,	0.012431,	0.0114861,	0.0106781,	0.00984938,	0.00909871,
    0.00843331,	 0.00785657,	0.00723358,	0.0066419,	0.00599867,	0.00563297,	0.00518391,	0.004678,	0.00437831,	0.00410608,
    0.00364064,	 0.00340744,	0.00303836,	0.00285093,	0.0026134,	0.00236285,	0.00221879,	0.00197884,	0.00180586,	0.00173118,
    0.00154375,	 0.00147389,	0.00136018,	0.0012378,	0.00110433,	0.0010099,	0.000963642,	0.000868241,	0.000836923,	0.000752604,
    0.000649495, 0.000618176,	0.000603722,	0.000543976,	0.000476521,	0.000447612,	0.000396057,	0.000378229,	0.000363293,	0.000354138,
    0.000287647, 0.000282347,	0.00025922,	0.000260183,	0.000238983,	0.000201883,	0.000204292,	0.000198992,	0.000176828,	0.000161892,
    0.00013491,	 0.00014551,	0.000106964,	0.000112264,	0.000101664,	0.00010311,	9.29914e-05,	7.8055e-05,	7.8055e-05,	6.69731e-05,
    7.08277e-05, 5.49276e-05,	6.07094e-05,	6.79367e-05,	4.77003e-05,	4.77003e-05,	4.19184e-05,	3.71002e-05,	4.43275e-05,	3.13184e-05,
    2.98729e-05, 3.66184e-05,	2.60183e-05,	2.89093e-05,	2.55365e-05,	2.55365e-05,	2.45729e-05,	2.12001e-05,	2.16819e-05,	2.65001e-05
  };

  //The array is binned in intervals of 5 GeV
  int ipt = (int) pt / 5;

  double pt_prob = 0;

  if (ipt > 119) pt_prob = 2E-5;
  else           pt_prob = prob_array[ipt];

  return pt_prob;
}




void AMWT::smear_JetMET(const vector <TLorentzVector> & orig_jets, const TVector2 & orig_met,
		vector <TLorentzVector> & smear_jets, TVector2 & smear_met,
		TRandom3* rand3, const vector <TF1*> & vPtRes, const vector <TF1*> & vEtaRes,
		const vector <TF1*> & vPhiRes, const TLorentzVector & lep_sum) const{

  smear_jets.clear();
  
  double sum_jpx = 0;
  double sum_jpy = 0;
  
  double sum_jpx_sm = 0;
  double sum_jpy_sm = 0;

  double Pt_sm, Eta_sm, Phi_sm;

  TLorentzVector v_temp;
  Long64_t iseed = (Long64_t) 10E10;

  for (unsigned int sui = 0; sui < orig_jets.size(); sui++){

//     a = 1.84; b = 1.14; c = 0.027;
    
//     if (orig_jets.at(sui).Eta() < 1.4){
//       a = 1.78; b = 1.3; c = 0.053;
//     }

//     Et = orig_jets.at(sui).Pt();
    
//     Pt_sm = Et + sqrt(a*a + b*b*Et + c*c*Et*Et) * rand3->Gaus();

//     Eta_sm = orig_jets.at(sui).Eta();
//     Phi_sm = orig_jets.at(sui).Phi();

    
    gRandom->SetSeed(rand3->Integer(iseed));
//     Pt_sm  = orig_jets.at(sui).Pt()  * (1 + (vPtRes.at(sui)->GetRandom() - 1) * 1.1);
    Pt_sm  = orig_jets.at(sui).Pt()  * vPtRes.at(sui)->GetRandom();

    gRandom->SetSeed(rand3->Integer(iseed));
    Eta_sm = orig_jets.at(sui).Eta() + vEtaRes.at(sui)->GetRandom();
    
    gRandom->SetSeed(rand3->Integer(iseed));
    Phi_sm = orig_jets.at(sui).Phi() + vPhiRes.at(sui)->GetRandom();
    
    v_temp.SetPtEtaPhiM(Pt_sm, Eta_sm, Phi_sm, orig_jets.at(sui).M());
   
    sum_jpx += orig_jets.at(sui).Px();
    sum_jpy += orig_jets.at(sui).Py();

    sum_jpx_sm += v_temp.Px();
    sum_jpy_sm += v_temp.Py();

    smear_jets.push_back(v_temp);
  }

  double unclust_metx = orig_met.Px() + sum_jpx + lep_sum.Px();
  double unclust_mety = orig_met.Py() + sum_jpy + lep_sum.Py();

  //10% resolution
  double unclust_metx_sm = unclust_metx * (1 + 0.1*rand3->Gaus());
  double unclust_mety_sm = unclust_mety * (1 + 0.1*rand3->Gaus());

  smear_met.Set(orig_met.Px() + sum_jpx - unclust_metx - sum_jpx_sm + unclust_metx_sm, orig_met.Py() + sum_jpy - unclust_mety - sum_jpy_sm + unclust_mety_sm);
}

void AMWT::dont_smear_JetMET(const vector <TLorentzVector> & orig_jets,
	const TVector2 & orig_met, vector <TLorentzVector> & smear_jets,
	TVector2 & smear_met) const
{ 
  smear_met  = orig_met;
  smear_jets = orig_jets;
}

