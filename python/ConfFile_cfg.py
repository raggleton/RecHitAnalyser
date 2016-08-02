import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:bad_RECO_C.root',
        'file:bad_RECO_D.root'
        # 'file:good_RECO.root'
    )
)

process.recHitAnalyser = cms.EDAnalyzer('RecHitAnalyser',
    recHitTokenHBHE=cms.untracked.InputTag("hbhereco", ""),
    recHitTokenHF=cms.untracked.InputTag("hfreco", ""),
    pfRecHitTokenHBHE=cms.untracked.InputTag("particleFlowRecHitHBHE", "Cleaned"),
    pfRecHitTokenHF=cms.untracked.InputTag("particleFlowRecHitHF", "Cleaned")
)


process.p = cms.Path(process.recHitAnalyser)
process.endjob_step = cms.EndPath(process.endOfProcess)

process.schedule = cms.Schedule(process.p, process.endjob_step)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("rechits_bad.root")
)