# import skeleton process
#from PhysicsTools.PatAlgos.patTemplate_cfg import *


import FWCore.ParameterSet.Config as cms

process = cms.Process("StandAlone")

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.load("Configuration.StandardSequences.MagneticField_cff")


process.load("PhysicsTools.PatAlgos.patSequences_cff")

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


## Output Module Configuration (expects a path 'p')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('miniTreeTuple.root'),
                               outputCommands = cms.untracked.vstring('drop *', 'keep TopTreeMTEvent_*_*_StandAlone' )
                               )

process.outpath = cms.EndPath(process.out)



process.source = cms.Source(
	"PoolSource",
	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring(
	       'file:patTuple.root'
	)
)



process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
		isAOD 	            = cms.bool(True), # true if processing AOD data
		isData 	            = cms.bool(False), # true if processing AOD data
		verbose 	    = cms.int32(1), #0: nothing - >1 gradually more information
	        TriggerMenu         = cms.InputTag("TriggerResults::REDIGI311X"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
		TriggerInfo         = cms.bool(True), 
                saveAllTriggers     = cms.bool(True),#should be True by default !!
		triggerList	    = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
		doGenParticleCollection = cms.bool(True),
 		doElectrons	    = cms.bool(True),
		electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
		electron_cut_pt     = cms.double(10),
		electron_cut_eta    = cms.double(2.5), 
 		doPhotons	    = cms.bool(False),
		photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
		photon_cut_pt       = cms.double(10),
		photon_cut_eta      = cms.double(2.5), 
		doMuons		    = cms.bool(True),
		muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
		muon_cut_pt         = cms.double(10),
		muon_cut_eta        = cms.double(2.5), 
		doTaus 		    = cms.bool(False),
		tauProducer         = cms.vstring("selectedPatTaus","selectedPatTausHpsPFTau"),
		#tauProducer         = cms.vstring("selectedPatTaus"),
		tau_cut_pt 	    = cms.double(10),
		tau_cut_eta	    = cms.double(2.4),      
		trackProducer       = cms.InputTag("generalTracks"),
		doTracks            = cms.bool(False),
                track_cut_pt        = cms.double(0.5),
                track_cut_eta       = cms.double(2.4),
		beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
		jet_cut_pt	    = cms.double(10),
		jet_cut_eta	    = cms.double(2.5), 
		met_cut_pt          = cms.double(-99),		
		saveAllVertex	    = cms.bool(True),
		vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
	        jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("ak5")),
		                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
		                                cms.PSet(jet = cms.untracked.string("selectedPatJets"),       met = cms.untracked.string("patMETs"),       algo =cms.untracked.string("ak5"))
		                             )
	)




process.p = cms.Path(process.MiniTreeProduction)








