#include "../interface/NTJetMet.h"
using namespace TopTree;
using namespace std;

NTJetMet::NTJetMet ():
sumEtJet (-999.), sumEtJetRaw (-999.)
{
}

NTJetMet::~NTJetMet ()
{
}

void
NTJetMet::Reset ()
{
  NTCollection<NTJet>::Reset ();
  met.Reset ();
  sumEtJet = -999.;
  sumEtJetRaw = -999.;
  
}
