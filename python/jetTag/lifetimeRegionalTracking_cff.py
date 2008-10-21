import FWCore.ParameterSet.Config as cms
import copy

from RecoTracker.Configuration.RecoTracker_cff import *

from TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi import *
bJetRegionalTrajectoryFilter = copy.deepcopy(trajectoryFilterESProducer)
bJetRegionalTrajectoryFilter.ComponentName = 'bJetRegionalTrajectoryFilter'
bJetRegionalTrajectoryFilter.filterPset.minPt = 1.0
bJetRegionalTrajectoryFilter.filterPset.maxNumberOfHits = 8

from RecoTracker.CkfPattern.CkfTrajectoryBuilderESProducer_cfi import *
bJetRegionalTrajectoryBuilder = copy.deepcopy(CkfTrajectoryBuilder)
bJetRegionalTrajectoryBuilder.ComponentName = 'bJetRegionalTrajectoryBuilder'
bJetRegionalTrajectoryBuilder.trajectoryFilterName = 'bJetRegionalTrajectoryFilter'
bJetRegionalTrajectoryBuilder.maxCand = 1
bJetRegionalTrajectoryBuilder.alwaysUseInvalidHits = False

from RecoTracker.CkfPattern.CkfTrackCandidates_cfi import *
hltBLifetimeRegionalCkfTrackCandidates = copy.deepcopy(ckfTrackCandidates)
hltBLifetimeRegionalCkfTrackCandidates.src = cms.InputTag('hltBLifetimeRegionalPixelSeedGenerator')
hltBLifetimeRegionalCkfTrackCandidates.TrajectoryBuilder = 'bJetRegionalTrajectoryBuilder'

from RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi import *
hltBLifetimeRegionalCtfWithMaterialTracks = copy.deepcopy(ctfWithMaterialTracks)
hltBLifetimeRegionalCtfWithMaterialTracks.src = 'hltBLifetimeRegionalCkfTrackCandidates'

hltBLifetimeRegionalPixelSeedGenerator = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.25),
            originHalfLength = cms.double(0.2),
            originZPos = cms.double(0.0),
            deltaEtaRegion = cms.double(0.25),
            ptMin = cms.double(1.0),
            JetSrc = cms.InputTag("hltBLifetimeL3Jets"),
            originRadius = cms.double(0.2),
            vertexSrc = cms.InputTag("pixelVertices")
        )
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)

hltBLifetimeRegionalTracks = cms.Sequence(
    hltBLifetimeRegionalPixelSeedGenerator * 
    hltBLifetimeRegionalCkfTrackCandidates * 
    hltBLifetimeRegionalCtfWithMaterialTracks)

hltBLifetimeL3tracking = cms.Sequence(
    cms.SequencePlaceholder("doLocalPixel") + 
    cms.SequencePlaceholder("doLocalStrip") * 
    hltBLifetimeRegionalTracks)
