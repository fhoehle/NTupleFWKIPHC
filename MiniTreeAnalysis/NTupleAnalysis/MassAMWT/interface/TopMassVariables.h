#ifndef TopMassVariables_h
#define TopMassVariables_h

#include "../../Selection/interface/DiLeptonSelection.h"
#include <TLorentzVector.h>
#include <vector>
#include <map>

struct ComparePt{
  bool operator()(const TLorentzVector & v1, const TLorentzVector & v2) const {
    return v1.Pt() > v2.Pt();
  }
  bool operator()(const NTJet & v1, const NTJet & v2) const {
    return v1.p4.Pt() > v2.p4.Pt();
  }
};


class TopMassVariables{

public:

  TopMassVariables( vector<NTJet> & selJet, vector<NTElectron> & selElectron, vector<NTMuon> & selMuon, 
  NTMET & selMET, string setCandPairType, const DiLeptonSelection & sel );
  
  TopMassVariables(const DiLeptonSelection & sel);
  
  
  
  ~TopMassVariables();

  void setMass(TH1F* massWeightHisto);

  void printAll() const;

  float peakMass() const {return peakMass_;}
  float peakWeight() const {return weight_;}
  TH1F* massWeightHisto() {return massWeightHisto_;}
  bool hasMassWeightHisto() const {return hasMassWeightHisto_;}
  bool hasMassSolution() const {return hasMassSolution_;}

  TLorentzVector const& posLeptonLV() const {return lep_p;}
  TLorentzVector const& negLeptonLV() const {return lep_m;}
  TLorentzVector const& diLeptonLV()  const {return lep_sum;}

  /**
   *   Get LV of selected jet -- starts at 0
   */
  TLorentzVector const& selectedJetLV(const int i) const {return (i==0?jet1:jet2);}
  bool selectedJetTag(const int i) const {return (i==0?jetTagged[sel_jet1]:jetTagged[sel_jet2]);}
  unsigned int nJets() const {return v_jets.size();}
  unsigned int nBJets() const {return nBjets;}
  

  TVector2 const& metV()  const {return v_met;}
  float hT() const {return HT;}

  bool isEE() const {return b_good_EE;}
  bool isEM() const {return b_good_EM;}
  bool isMM() const {return b_good_MM;}

  string const& dileptonType() const {return candPairType;}

  float top1Pt() const {return top1Pt_;}
  float top2Pt() const {return top2Pt_;}
  float tTbarPt() const {return tTbarPt_;}
  void setTopPt(float top1Pt, float top2Pt, float tTbarPt)
	{top1Pt_ = top1Pt; top2Pt_ = top2Pt; tTbarPt_ = tTbarPt;}
	
  void setTop1P4(float top1Px, float top1Py, float top1Pz, float top1E)
	{Top1P4.SetPxPyPzE(top1Px, top1Py, top1Pz, top1E);}
	
  void setTop2P4(float top2Px, float top2Py, float top2Pz, float top2E)
	{Top2P4.SetPxPyPzE(top2Px, top2Py, top2Pz, top2E);}

  TLorentzVector getTop1P4(){return Top1P4;}
  TLorentzVector getTop2P4(){return Top2P4;}

private:

  void Print4Vector(const TLorentzVector & v1) const;

  ComparePt ptComparator;
  float peakMass_;
  float weight_;
  TH1F* massWeightHisto_;
  bool hasMassWeightHisto_, hasMassSolution_;
  unsigned int sel_jet1, sel_jet2;

  TLorentzVector lep_p;
  TLorentzVector lep_m;
  TLorentzVector lep_sum;

  vector <NTJet> v_jets;
  vector <bool> jetTagged;
  TLorentzVector jet1, jet2;

  TVector2 v_met;
  float HT;
  int nBjets;

  bool b_good_EE;
  bool b_good_EM;
  bool b_good_MM;

  string candPairType;

  float top1Pt_;
  float top2Pt_;
  float tTbarPt_;
  
  TLorentzVector Top1P4;
  TLorentzVector Top2P4;
  

};
#endif
