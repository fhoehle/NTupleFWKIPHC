#ifndef AMWT_h
#define AMWT_h

#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Selection/interface/DiLeptonSelection.h"
#include "./MassSolver.h"
#include "./TopMassVariables.h"

// system include files
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include <TH1.h>
#include <TRandom3.h>

#include "../../../../JR_Standalone/JetMETObjects/interface/JetResolution.h"

using namespace std;

class AMWT{

  public:
    AMWT(AnalysisEnvironmentLoader & anaEL, bool isData);
    ~AMWT();
    TopMassVariables * findMass(const DiLeptonSelection & sel);
    TopMassVariables * findMass(TopMassVariables & topMassVariables);

   int massPoints() const {return nMassPts;}
   int massRangeMin() const {return rangeLow;}
   int massRangeMax() const {return rangeHigh;}
   int nbrJetSmearIterations() const {return nbrJetSmear;}
   TH1F * sampleWeightHisto() const;


  private:
    //bool fexists(const string filename, bool fail);

    double get_dalitz_prob(const TLorentzVector & lep, const TLorentzVector & top,
		double mb, double mw) const;
    double get_weight(const TLorentzVector & bquark1, const TLorentzVector & bquark2,
    		const TLorentzVector & lep_p, const TLorentzVector & lep_m,
		const TLorentzVector & nu1, const TLorentzVector & nu2,
		const double top_mass) const;
    double get_top_pt_prob(const double pt) const;
    double get_2bjet_prob(const TLorentzVector & jet1, const TLorentzVector & jet2,
		map <double,double> & mapJetPhi2Discr) const;

    void smear_JetMET(const vector <TLorentzVector> & orig_jets, const TVector2 & orig_met,
		vector <TLorentzVector> & smear_jets, TVector2 & smear_met,
		TRandom3* rand3, const vector <TF1*> & vPtRes, const vector <TF1*> & vEtaRes,
		const vector <TF1*> & vPhiRes, const TLorentzVector & lep_sum) const;
    void dont_smear_JetMET(const vector <TLorentzVector> & orig_jets, const TVector2 & orig_met,
		vector <TLorentzVector> & smear_jets, TVector2 & smear_met) const;


    int nMassPts;
    int rangeLow, rangeHigh;
    int nbrJetSmear;
    float Luminosity;
    // 0: MC - 1: Data - 2 Data & MC
    int DataType;
    int verbosity;


    MassSolver * massSolver;
    TRandom3* rand3;

    JetResolution* ptResol;
    JetResolution* etaResol;
    JetResolution* phiResol;
};

#endif
