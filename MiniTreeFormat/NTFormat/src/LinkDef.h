#ifdef __CINT__
#include  "../interface/NTLepton.h"
#include  "../interface/NTMuon.h"
#include  "../interface/NTElectron.h"
#include  "../interface/NTPhoton.h"
#include  "../interface/NTTau.h"
#include  "../interface/NTTrack.h"
#include  "../interface/NTVertex.h"
#include  "../interface/NTJet.h"
#include  "../interface/NTMET.h"
#include  "../interface/NTJetMet.h"
#include  "../interface/WDecaysMC.h"
#include  "../interface/ZDecaysMC.h"
#include  "../interface/topDecaysMC.h"
#include  "../interface/NTEvent.h"
#include  "../interface/NTCollection.h"
#include  "../interface/NTPU.h"
#else
#include  "../interface/NTLepton.h"
#include  "../interface/NTMuon.h"
#include  "../interface/NTElectron.h"
#include  "../interface/NTPhoton.h"
#include  "../interface/NTTau.h"
#include  "../interface/NTTrack.h"
#include  "../interface/NTVertex.h"
#include  "../interface/NTJet.h"
#include  "../interface/NTMET.h"
#include  "../interface/NTJetMet.h"
#include  "../interface/WDecaysMC.h"
#include  "../interface/ZDecaysMC.h"
#include  "../interface/topDecaysMC.h"
#include  "../interface/NTEvent.h"
#include  "../interface/NTCollection.h"
#include  "../interface/NTPU.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TopTree::NTLepton+;
#pragma link C++ class TopTree::NTMuon+;
#pragma link C++ class TopTree::NTElectron+;
#pragma link C++ class TopTree::NTPhoton+;
#pragma link C++ class TopTree::NTTau+;
#pragma link C++ class TopTree::NTTrack+;
#pragma link C++ class TopTree::NTVertex+;
#pragma link C++ class TopTree::NTJet+;
#pragma link C++ class TopTree::NTMET+;
#pragma link C++ class TopTree::NTJetMet+;
#pragma link C++ class TopTree::NTCollection<TopTree::NTTau>+;
#pragma link C++ class TopTree::NTCollection<TopTree::NTJet>+;
#pragma link C++ class TopTree::NTEvent+;
#pragma link C++ class TopTree::WDecaysMC+;
#pragma link C++ class TopTree::ZDecaysMC+;
#pragma link C++ class TopTree::TopDecaysMC+;
#pragma link C++ class std::pair<std::string,bool>+;
#pragma link C++ class TopTree::NTPU+;

#endif
