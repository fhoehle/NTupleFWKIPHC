#ifndef __TOP_TREE_ZDECAY_EVTMC_H__
#define __TOP_TREE_ZDECAY_EVTMC_H__

#include <vector>
#include "TLorentzVector.h"

//using namespace std;

namespace TopTree
{

  class ZDecaysMC
  {

  public:

    //methods
    ZDecaysMC (){};
    ~ZDecaysMC (){};

    //data memebers  
    TLorentzVector p4_Z_gen;
    TLorentzVector p4_Lep1_gen;
    TLorentzVector p4_Lep2_gen;
    TLorentzVector p4_SumTauNeu_gen;
    TLorentzVector p4_SumTauANeu_gen;
    double Q_Lep1_gen;
    double Q_Lep2_gen;
    int Lep1_pdgID;
    int Lep2_pdgID;
    int Tmeme;
    int Zgtobb;
  
  
  

  };
}




#endif				//__TOP_TREE_ZDECAY_PARTON_EVT_H__
