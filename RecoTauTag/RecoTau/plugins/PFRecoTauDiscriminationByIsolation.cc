#include <functional>
#include <boost/foreach.hpp>
#include "RecoTauTag/RecoTau/interface/TauDiscriminationProducerBase.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "RecoTauTag/RecoTau/interface/RecoTauQualityCuts.h"
#include "RecoTauTag/RecoTau/interface/RecoTauVertexAssociator.h"
#include "RecoTauTag/RecoTau/interface/ConeTools.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TMath.h"
#include "TFormula.h"

/* class PFRecoTauDiscriminationByIsolation
 * created : Jul 23 2007,
 * revised : Thu Aug 13 14:44:40 PDT 2009
 * contributors : Ludovic Houchu (IPHC, Strasbourg),
 *               Christian Veelken (UC Davis),
*                Evan K. Friis (UC Davis)
 *               Michalis Bachtis (UW Madison)
 */

using namespace reco;
using namespace std;

class PFRecoTauDiscriminationByIsolation :
  public PFTauDiscriminationProducerBase  {
  public:
    explicit PFRecoTauDiscriminationByIsolation(const edm::ParameterSet& pset):
      PFTauDiscriminationProducerBase(pset),
      qualityCutsPSet_(pset.getParameter<edm::ParameterSet>("qualityCuts")) {

        includeTracks_ = pset.getParameter<bool>(
            "ApplyDiscriminationByTrackerIsolation");
        includeGammas_ = pset.getParameter<bool>(
            "ApplyDiscriminationByECALIsolation");

        applyOccupancyCut_ = pset.getParameter<bool>("applyOccupancyCut");
        maximumOccupancy_ = pset.getParameter<uint32_t>("maximumOccupancy");

        applySumPtCut_ = pset.getParameter<bool>("applySumPtCut");
        maximumSumPt_ = pset.getParameter<double>("maximumSumPtCut");

        applyRelativeSumPtCut_ = pset.getParameter<bool>(
            "applyRelativeSumPtCut");
        maximumRelativeSumPt_ = pset.getParameter<double>(
            "relativeSumPtCut");

        if (pset.exists("customOuterCone")) {
          customIsoCone_ = pset.getParameter<double>("customOuterCone");
        } else {
          customIsoCone_ = -1;
        }

        // Check for legacy HLT configuration and apply workaround. FIXME
        // This should be removed once the HLT configs are updated.
        if (pset.exists("PVProducer")) {
          edm::LogWarning("DeprecationOfParameter") << "The parameter "
            << "PVProducer of PFRecoTauDiscriminationByIsolation "
            << "is deprecated, and should be moved to the qualityCuts "
            << "configuration PSet.  A workaround is being applied, but please "
            << "update your configuration.  The vertex selected as the primary "
            << "vertex will be the one with the highest pt in the event."
            << std::endl;
          // Move the selected PV collection to the new location
          if (qualityCutsPSet_.exists("primaryVertexSrc")) {
            edm::LogWarning("DeprecationOfParameter") << "Overwriting existing "
              << "PV source: " << qualityCutsPSet_.getParameter<edm::InputTag>(
                  "primaryVertexSrc")
              << " in the quality cut PSet!" << std::endl;
          }
          qualityCutsPSet_.addParameter<edm::InputTag>("primaryVertexSrc",
              pset.getParameter<edm::InputTag>("PVProducer"));
          qualityCutsPSet_.addParameter<std::string>("pvFindingAlgo",
              "highestPtInEvent");
        }

        // Get the quality cuts specific to the isolation region
        edm::ParameterSet isolationQCuts = qualityCutsPSet_.getParameterSet(
            "isolationQualityCuts");

        qcuts_.reset(new tau::RecoTauQualityCuts(isolationQCuts));

        vertexAssociator_.reset(
            new tau::RecoTauVertexAssociator(qualityCutsPSet_));

        applyDeltaBeta_ = false;
        if (pset.exists("applyDeltaBetaCorrection")) {
          // Factorize the isolation QCuts into those that are used to
          // select PU and those that are not.
          std::pair<edm::ParameterSet, edm::ParameterSet> puFactorizedIsoQCuts =
            reco::tau::factorizePUQCuts(isolationQCuts);

          // Determine the pt threshold for the PU tracks
          // First check if the user specifies explicitly the cut.
          if (pset.exists("deltaBetaPUTrackPtCutOverride")) {
            puFactorizedIsoQCuts.second.addParameter<double>(
                "minTrackPt",
                pset.getParameter<double>("deltaBetaPUTrackPtCutOverride"));
          } else {
            // Secondly take it from the minGammaEt
            puFactorizedIsoQCuts.second.addParameter<double>(
                "minTrackPt",
                isolationQCuts.getParameter<double>("minGammaEt"));
          }

          pileupQcutsPUTrackSelection_.reset(new tau::RecoTauQualityCuts(
                puFactorizedIsoQCuts.first));

          pileupQcutsGeneralQCuts_.reset(new tau::RecoTauQualityCuts(
                puFactorizedIsoQCuts.second));

          applyDeltaBeta_ = pset.getParameter<bool>(
              "applyDeltaBetaCorrection");
          pfCandSrc_ = pset.getParameter<edm::InputTag>("particleFlowSrc");
          vertexSrc_ = pset.getParameter<edm::InputTag>("vertexSrc");
          deltaBetaCollectionCone_ = pset.getParameter<double>(
              "isoConeSizeForDeltaBeta");
          std::string deltaBetaFactorFormula =
            pset.getParameter<string>("deltaBetaFactor");
          deltaBetaFormula_.reset(
              new TFormula("DB_corr", deltaBetaFactorFormula.c_str()));
        }

        applyRhoCorrection_ = pset.exists("applyRhoCorrection") ?
          pset.getParameter<bool>("applyRhoCorrection") : false;
        if (applyRhoCorrection_) {
          rhoProducer_ = pset.getParameter<edm::InputTag>("rhoProducer");
          rhoConeSize_ = pset.getParameter<double>("rhoConeSize");
          rhoUEOffsetCorrection_ =
            pset.getParameter<double>("rhoUEOffsetCorrection");
        }
      }

    ~PFRecoTauDiscriminationByIsolation(){}

    void beginEvent(const edm::Event& evt, const edm::EventSetup& evtSetup);
    double discriminate(const PFTauRef& pfTau);

  private:
    edm::ParameterSet qualityCutsPSet_;
    std::auto_ptr<tau::RecoTauQualityCuts> qcuts_;

    // Inverted QCut which selects tracks with bad DZ/trackWeight
    std::auto_ptr<tau::RecoTauQualityCuts> pileupQcutsPUTrackSelection_;
    std::auto_ptr<tau::RecoTauQualityCuts> pileupQcutsGeneralQCuts_;

    std::auto_ptr<tau::RecoTauVertexAssociator> vertexAssociator_;

    bool includeTracks_;
    bool includeGammas_;
    bool applyOccupancyCut_;
    uint32_t maximumOccupancy_;
    bool applySumPtCut_;
    double maximumSumPt_;
    bool applyRelativeSumPtCut_;
    double maximumRelativeSumPt_;
    double customIsoCone_;


    // PU subtraction parameters

    // Delta Beta correction
    bool applyDeltaBeta_;
    edm::InputTag pfCandSrc_;
    // Keep track of how many vertices are in the event
    edm::InputTag vertexSrc_;
    std::vector<reco::PFCandidateRef> chargedPFCandidatesInEvent_;
    // Size of cone used to collect PU tracks
    double deltaBetaCollectionCone_;
    std::auto_ptr<TFormula> deltaBetaFormula_;
    double deltaBetaFactorThisEvent_;

    // Rho correction
    bool applyRhoCorrection_;
    edm::InputTag rhoProducer_;
    double rhoConeSize_;
    double rhoUEOffsetCorrection_;
    double rhoCorrectionThisEvent_;
    double rhoThisEvent_;
  };

void PFRecoTauDiscriminationByIsolation::beginEvent(const edm::Event& event,
    const edm::EventSetup& eventSetup) {

  // NB: The use of the PV in this context is necessitated by its use in
  // applying quality cuts to the different objects in the isolation cone
  // The vertex associator contains the logic to select the appropriate vertex
  // We need to pass it the event so it can load the vertices.
  vertexAssociator_->setEvent(event);

  // If we are applying the delta beta correction, we need to get the PF
  // candidates from the event so we can find the PU tracks.
  chargedPFCandidatesInEvent_.clear();
  if (applyDeltaBeta_) {
    // Collect all the PF pile up tracks
    edm::Handle<reco::PFCandidateCollection> pfCandHandle_;
    event.getByLabel(pfCandSrc_, pfCandHandle_);
    chargedPFCandidatesInEvent_.reserve(pfCandHandle_->size());
    for (size_t i = 0; i < pfCandHandle_->size(); ++i) {
      reco::PFCandidateRef pfCand(pfCandHandle_, i);
      if (pfCand->charge() != 0)
        chargedPFCandidatesInEvent_.push_back(pfCand);
    }
    // Count all the vertices in the event, to parameterize the DB
    // correction factor
    edm::Handle<reco::VertexCollection> vertices;
    event.getByLabel(vertexSrc_, vertices);
    size_t nVtxThisEvent = vertices->size();
    deltaBetaFactorThisEvent_ = deltaBetaFormula_->Eval(nVtxThisEvent);
  }

  if (applyRhoCorrection_) {
    edm::Handle<double> rhoHandle_;
    event.getByLabel(rhoProducer_, rhoHandle_);
    rhoThisEvent_ = (*rhoHandle_ - rhoUEOffsetCorrection_)*
      (3.14159)*rhoConeSize_*rhoConeSize_;
  }
}

double
PFRecoTauDiscriminationByIsolation::discriminate(const PFTauRef& pfTau) {
  // collect the objects we are working with (ie tracks, tracks+gammas, etc)
  std::vector<PFCandidateRef> isoCharged;
  std::vector<PFCandidateRef> isoNeutral;
  std::vector<PFCandidateRef> isoPU;

  // Get the primary vertex associated to this tau
  reco::VertexRef pv = vertexAssociator_->associatedVertex(*pfTau);
  // Let the quality cuts know which the vertex to use when applying selections
  // on dz, etc.
  qcuts_->setPV(pv);
  if (applyDeltaBeta_) {
    pileupQcutsGeneralQCuts_->setPV(pv);
    pileupQcutsPUTrackSelection_->setPV(pv);
  }
  // Load the tracks if they are being used.
  if (includeTracks_) {
    BOOST_FOREACH(const reco::PFCandidateRef& cand,
        pfTau->isolationPFChargedHadrCands()) {
      if (qcuts_->filterRef(cand))
        isoCharged.push_back(cand);
    }
  }

  if (includeGammas_) {
    BOOST_FOREACH(const reco::PFCandidateRef& cand,
        pfTau->isolationPFGammaCands()) {
      if (qcuts_->filterRef(cand))
        isoNeutral.push_back(cand);
    }
  }
  typedef reco::tau::cone::DeltaRPtrFilter<PFCandidateRef> DRFilter;

  // If desired, get PU tracks.
  if (applyDeltaBeta_) {
    // First select by inverted the DZ/track weight cuts. True = invert
    //std::cout << "Initial PFCands: " << chargedPFCandidatesInEvent_.size()
    //  << std::endl;

    std::vector<PFCandidateRef> allPU =
      pileupQcutsPUTrackSelection_->filterRefs(
          chargedPFCandidatesInEvent_, true);

    //std::cout << "After track cuts: " << allPU.size() << std::endl;

    // Now apply the rest of the cuts, like pt, and TIP, tracker hits, etc
    std::vector<PFCandidateRef> cleanPU =
      pileupQcutsGeneralQCuts_->filterRefs(allPU);

    //std::cout << "After cleaning cuts: " << cleanPU.size() << std::endl;

    // Only select PU tracks inside the isolation cone.
    DRFilter deltaBetaFilter(pfTau->p4(), 0, deltaBetaCollectionCone_);
    BOOST_FOREACH(const reco::PFCandidateRef& cand, cleanPU) {
      if (deltaBetaFilter(cand)) {
        isoPU.push_back(cand);
      }
    }
    //std::cout << "After cone cuts: " << isoPU.size() << std::endl;
  }

  // Check if we want a custom iso cone
  if (customIsoCone_ >= 0.) {
    DRFilter filter(pfTau->p4(), 0, customIsoCone_);
    // Remove all the objects not in our iso cone
    std::remove_if(isoCharged.begin(), isoCharged.end(), std::not1(filter));
    std::remove_if(isoNeutral.begin(), isoNeutral.end(), std::not1(filter));
    std::remove_if(isoPU.begin(), isoPU.end(), std::not1(filter));
  }

  bool failsOccupancyCut     = false;
  bool failsSumPtCut         = false;
  bool failsRelativeSumPtCut = false;

  //--- nObjects requirement
  int neutrals = isoNeutral.size();

  if (applyDeltaBeta_) {
    neutrals -= TMath::Nint(deltaBetaFactorThisEvent_*isoPU.size());
  }
  if(neutrals < 0) {
    neutrals=0;
  }

  failsOccupancyCut = ( isoCharged.size()+neutrals > maximumOccupancy_ );

  //--- Sum PT requirement
  if( applySumPtCut_ || applyRelativeSumPtCut_ ) {
    double totalPt=0.0;
    double chargedPt=0.0;
    double puPt=0.0;
    double neutralPt=0.0;
    BOOST_FOREACH(const PFCandidateRef& isoObject, isoCharged) {
      chargedPt += isoObject->pt();
    }
    BOOST_FOREACH(const PFCandidateRef& isoObject, isoNeutral) {
      neutralPt += isoObject->pt();
    }
    BOOST_FOREACH(const PFCandidateRef& isoObject, isoPU) {
      puPt += isoObject->pt();
    }

    if (applyDeltaBeta_) {
      neutralPt -= deltaBetaFactorThisEvent_*puPt;
    }

    if (applyRhoCorrection_) {
      neutralPt -= rhoThisEvent_;
    }

    if (neutralPt < 0.0) {
      neutralPt = 0.0;
    }

    totalPt = chargedPt+neutralPt;

    failsSumPtCut = (totalPt > maximumSumPt_);

    //--- Relative Sum PT requirement
    failsRelativeSumPtCut = (
        (pfTau->pt() > 0 ? totalPt/pfTau->pt() : 0 ) > maximumRelativeSumPt_ );
  }

  bool fails = (applyOccupancyCut_ && failsOccupancyCut) ||
    (applySumPtCut_ && failsSumPtCut) ||
    (applyRelativeSumPtCut_ && failsRelativeSumPtCut) ;

  return (fails ? 0. : 1.);
}

DEFINE_FWK_MODULE(PFRecoTauDiscriminationByIsolation);
