#include "../interface/MTJetMet.h"

using namespace TopTree;
using namespace std;
using namespace edm;
using namespace reco;

MTJetMet::MTJetMet ():
sumEtJet (-999.), sumEtJetRaw (-999.)
{
}

MTJetMet::~MTJetMet ()
{
}

void
MTJetMet::Reset ()
{
  MTCollection<MTJet>::Reset ();
  met.Reset ();
  sumEtJet = -999.;
  sumEtJetRaw = -999.;
  
}
