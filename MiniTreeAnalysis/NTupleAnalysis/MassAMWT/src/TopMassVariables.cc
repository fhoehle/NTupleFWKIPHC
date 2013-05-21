#include "../interface/TopMassVariables.h"
#include "../interface/Constants.h"

TopMassVariables::TopMassVariables( vector<NTJet> & selJet, vector<NTElectron> & selElectron, vector<NTMuon> & selMuon, 
  NTMET & selMET, string setCandPairType, const DiLeptonSelection & sel )
{
  hasMassSolution_ = false;
  hasMassWeightHisto_ = false;
  peakMass_ = 0.;
  weight_ = 0.;

 candPairType = setCandPairType;
  
  
  int nMu = selMuon.size();
  int nE = selElectron.size();

  b_good_EE = false;
  b_good_EM = false;
  b_good_MM = false;

  if (nE + nMu == 2) {
    if (nE == 2) {
      b_good_EE = true;
      candPairType = "ee";
      if (selElectron[0].Charge>0) {
	lep_p = selElectron[0].p4;
	lep_p.SetE(sqrt(selElectron[0].p4.Vect().Mag2()+m_electron*m_electron));
	lep_m = selElectron[1].p4;
	lep_m.SetE(sqrt(selElectron[1].p4.Vect().Mag2()+m_electron*m_electron));
      } else {
	lep_p = selElectron[1].p4;
	lep_p.SetE(sqrt(selElectron[1].p4.Vect().Mag2()+m_electron*m_electron));
	lep_m = selElectron[0].p4;
	lep_m.SetE(sqrt(selElectron[0].p4.Vect().Mag2()+m_electron*m_electron));
      }
    } else if (nMu == 2) {
      b_good_MM = true;
      candPairType = "mumu";
      if (selMuon[0].Charge>0) {
	lep_p = selMuon[0].p4;
	lep_p.SetE(sqrt(selMuon[0].p4.Vect().Mag2()+m_muon*m_muon));
	lep_m = selMuon[1].p4;
	lep_m.SetE(sqrt(selMuon[1].p4.Vect().Mag2()+m_muon*m_muon));
      } else {
	lep_p = selMuon[1].p4;
	lep_p.SetE(sqrt(selMuon[1].p4.Vect().Mag2()+m_muon*m_muon));
	lep_m = selMuon[0].p4;
	lep_m.SetE(sqrt(selMuon[0].p4.Vect().Mag2()+m_muon*m_muon));
      }
    } else if (nE == 1 && nMu == 1) {
      b_good_EM = true;
      candPairType = "emu";
      if (selMuon[0].Charge>0) {
	lep_p = selMuon[0].p4;
	lep_p.SetE(sqrt(selMuon[0].p4.Vect().Mag2()+m_muon*m_muon));
	lep_m = selElectron[0].p4;
	lep_m.SetE(sqrt(selElectron[0].p4.Vect().Mag2()+m_electron*m_electron));
      } else {
	lep_p = selElectron[0].p4;
	lep_p.SetE(sqrt(selElectron[0].p4.Vect().Mag2()+m_electron*m_electron));
	lep_m = selMuon[0].p4;
	lep_m.SetE(sqrt(selMuon[0].p4.Vect().Mag2()+m_muon*m_muon));
      }
    }
  }
  lep_sum = lep_m + lep_p;

  v_met.Set(selMET.p4.Px(), selMET.p4.Py());

  v_jets  = selJet;
  sort(v_jets.begin(), v_jets.end(), ptComparator);

  nBjets = 0;
  if (v_jets.size()>1) {
    sel_jet1 = 0; sel_jet2 = 1;

    for (unsigned int ibjet = 0; ibjet < v_jets.size(); ibjet++){
      if (sel.passBtagSelection(v_jets[ibjet])){
	++ nBjets;
	jetTagged.push_back(true);
	if ( nBjets == 1){
	  sel_jet1 = ibjet;
	  if (sel_jet1 != 0) sel_jet2 = 0;
	} else if ( nBjets == 2) sel_jet2 = ibjet;
      } else {
	jetTagged.push_back(false);
      }
    }

    jet1 = v_jets[sel_jet1].p4;
    jet2 = v_jets[sel_jet2].p4;

  } else {
    sel_jet1 = -1; sel_jet2 = -1;
    if (v_jets.size()==1) jetTagged.push_back(sel.passBtagSelection(v_jets[0]));
  }

  HT =  lep_p.Pt() + lep_m.Pt();

// 	 for(unsigned int i=0; i<candElec.size(); i++){
// 	   HT+=candElec[i].p4.Pt();
// 	 }
// 	 for(unsigned int i=0; i<candMuon.size(); i++){
// 	   HT+=candMuon[i].p4.Pt();
// 	 }
  for(unsigned int i=0; i<v_jets.size(); i++){
    HT+=v_jets[i].p4.Pt();
  }
  HT+=v_met.Mod();

  top1Pt_  = SENTINEL;
  top2Pt_  = SENTINEL;
  tTbarPt_ = SENTINEL;
}



TopMassVariables::TopMassVariables(const DiLeptonSelection & sel)
{
  hasMassSolution_ = false;
  hasMassWeightHisto_ = false;
  peakMass_ = 0.;
  weight_ = 0.;


  int nMu = sel.GetMuonsForAna().size();
  int nE = sel.GetElectronsForAna().size();

  b_good_EE = false;
  b_good_EM = false;
  b_good_MM = false;

  if (nE + nMu == 2) {
    if (nE == 2) {
      b_good_EE = true;
      candPairType = "ee";
      if (sel.GetElectronsForAna()[0].Charge>0) {
	lep_p = sel.GetElectronsForAna()[0].p4;
	lep_p.SetE(sqrt(sel.GetElectronsForAna()[0].p4.Vect().Mag2()+m_electron*m_electron));
	lep_m = sel.GetElectronsForAna()[1].p4;
	lep_m.SetE(sqrt(sel.GetElectronsForAna()[1].p4.Vect().Mag2()+m_electron*m_electron));
      } else {
	lep_p = sel.GetElectronsForAna()[1].p4;
	lep_p.SetE(sqrt(sel.GetElectronsForAna()[1].p4.Vect().Mag2()+m_electron*m_electron));
	lep_m = sel.GetElectronsForAna()[0].p4;
	lep_m.SetE(sqrt(sel.GetElectronsForAna()[0].p4.Vect().Mag2()+m_electron*m_electron));
      }
    } else if (nMu == 2) {
      b_good_MM = true;
      candPairType = "mumu";
      if (sel.GetMuonsForAna()[0].Charge>0) {
	lep_p = sel.GetMuonsForAna()[0].p4;
	lep_p.SetE(sqrt(sel.GetMuonsForAna()[0].p4.Vect().Mag2()+m_muon*m_muon));
	lep_m = sel.GetMuonsForAna()[1].p4;
	lep_m.SetE(sqrt(sel.GetMuonsForAna()[1].p4.Vect().Mag2()+m_muon*m_muon));
      } else {
	lep_p = sel.GetMuonsForAna()[1].p4;
	lep_p.SetE(sqrt(sel.GetMuonsForAna()[1].p4.Vect().Mag2()+m_muon*m_muon));
	lep_m = sel.GetMuonsForAna()[0].p4;
	lep_m.SetE(sqrt(sel.GetMuonsForAna()[0].p4.Vect().Mag2()+m_muon*m_muon));
      }
    } else if (nE == 1 && nMu == 1) {
      b_good_EM = true;
      candPairType = "emu";
      if (sel.GetMuonsForAna()[0].Charge>0) {
	lep_p = sel.GetMuonsForAna()[0].p4;
	lep_p.SetE(sqrt(sel.GetMuonsForAna()[0].p4.Vect().Mag2()+m_muon*m_muon));
	lep_m = sel.GetElectronsForAna()[0].p4;
	lep_m.SetE(sqrt(sel.GetElectronsForAna()[0].p4.Vect().Mag2()+m_electron*m_electron));
      } else {
	lep_p = sel.GetElectronsForAna()[0].p4;
	lep_p.SetE(sqrt(sel.GetElectronsForAna()[0].p4.Vect().Mag2()+m_electron*m_electron));
	lep_m = sel.GetMuonsForAna()[0].p4;
	lep_m.SetE(sqrt(sel.GetMuonsForAna()[0].p4.Vect().Mag2()+m_muon*m_muon));
      }
    }
  }
  lep_sum = lep_m + lep_p;

  v_met.Set(sel.GetMET().p4.Px(), sel.GetMET().p4.Py());

  v_jets  = sel.GetJetsForAna();
  sort(v_jets.begin(), v_jets.end(), ptComparator);

  nBjets = 0;
  if (v_jets.size()>1) {
    sel_jet1 = 0; sel_jet2 = 1;

    for (unsigned int ibjet = 0; ibjet < v_jets.size(); ibjet++){
      if (sel.passBtagSelection(v_jets[ibjet])){
	++ nBjets;
	jetTagged.push_back(true);
	if ( nBjets == 1){
	  sel_jet1 = ibjet;
	  if (sel_jet1 != 0) sel_jet2 = 0;
	} else if ( nBjets == 2) sel_jet2 = ibjet;
      } else {
	jetTagged.push_back(false);
      }
    }

    jet1 = v_jets[sel_jet1].p4;
    jet2 = v_jets[sel_jet2].p4;

  } else {
    sel_jet1 = -1; sel_jet2 = -1;
    if (v_jets.size()==1) jetTagged.push_back(sel.passBtagSelection(v_jets[0]));
  }

  HT =  lep_p.Pt() + lep_m.Pt();

// 	 for(unsigned int i=0; i<candElec.size(); i++){
// 	   HT+=candElec[i].p4.Pt();
// 	 }
// 	 for(unsigned int i=0; i<candMuon.size(); i++){
// 	   HT+=candMuon[i].p4.Pt();
// 	 }
  for(unsigned int i=0; i<v_jets.size(); i++){
    HT+=v_jets[i].p4.Pt();
  }
  HT+=v_met.Mod();

  top1Pt_  = SENTINEL;
  top2Pt_  = SENTINEL;
  tTbarPt_ = SENTINEL;
}

TopMassVariables::~TopMassVariables()
{
  if (hasMassSolution_) delete massWeightHisto_;
}

void TopMassVariables::setMass(TH1F* histo)
{
  massWeightHisto_ = histo;
  hasMassWeightHisto_ = true;
  hasMassSolution_ = true;
  weight_ = histo->GetBinContent(histo->GetMaximumBin());
  peakMass_  =   histo->GetXaxis()->GetBinLowEdge(histo->GetMaximumBin());
}

void TopMassVariables::printAll() const
{
  cout << "Got a "<<candPairType<<endl;
  Print4Vector(lep_p);cout <<endl;
  Print4Vector(lep_m);cout <<endl;
  cout << "Dilpeton invariant mass: "<< lep_sum.M()<<endl;
  cout << "Jets  / bJets: " << nJets() <<" / "<<nBJets()<<endl;

  for (unsigned int ijet = 0; ijet < v_jets.size(); ijet++){
    if (ijet == sel_jet1 || ijet == sel_jet2) cout<<"Selected ";
    else                                      cout<<"         ";
    cout<<"Jet "<<ijet<<": ";
    cout << (jetTagged[ijet]?"Tag   ":"NoTag ");
    Print4Vector(v_jets[ijet].p4);
    cout << endl;
  }

  if (hasMassSolution_) {
    cout <<"ttbar candidate with Mtop = "
      <<peakMass_<<" GeV with weight "
      <<weight_<<endl;

  }
}

void TopMassVariables::Print4Vector(const TLorentzVector & v1) const
{
  printf("| %8.2f | %8.2f | %8.2f | %8.2f |", v1.Pt(), v1.Eta(), v1.Phi(), v1.M());
}

