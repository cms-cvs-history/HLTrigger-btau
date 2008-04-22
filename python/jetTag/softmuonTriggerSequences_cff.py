import FWCore.ParameterSet.Config as cms

hltBSoftmuon1jet = cms.Sequence(cms.SequencePlaceholder("prescalerBSoftmuon1jet")+cms.SequencePlaceholder("hltBegin")+cms.SequencePlaceholder("hltBSoftmuonNjetL1seeds")+cms.SequencePlaceholder("hltBCommonL2reco")*cms.SequencePlaceholder("hltBSoftmuon1jetL2filter")+cms.SequencePlaceholder("hltBSoftmuonL25reco")*cms.SequencePlaceholder("hltBSoftmuonL25filter")+cms.SequencePlaceholder("hltBSoftmuonL3reco")*cms.SequencePlaceholder("hltBSoftmuonByDRL3filter"))
hltBSoftmuon2jet = cms.Sequence(cms.SequencePlaceholder("prescalerBSoftmuon2jet")+cms.SequencePlaceholder("hltBegin")+cms.SequencePlaceholder("hltBSoftmuonNjetL1seeds")+cms.SequencePlaceholder("hltBCommonL2reco")*cms.SequencePlaceholder("hltBSoftmuon2jetL2filter")+cms.SequencePlaceholder("hltBSoftmuonL25reco")*cms.SequencePlaceholder("hltBSoftmuonL25filter")+cms.SequencePlaceholder("hltBSoftmuonL3reco")*cms.SequencePlaceholder("hltBSoftmuonL3filter"))
hltBSoftmuon3jet = cms.Sequence(cms.SequencePlaceholder("prescalerBSoftmuon3jet")+cms.SequencePlaceholder("hltBegin")+cms.SequencePlaceholder("hltBSoftmuonNjetL1seeds")+cms.SequencePlaceholder("hltBCommonL2reco")*cms.SequencePlaceholder("hltBSoftmuon3jetL2filter")+cms.SequencePlaceholder("hltBSoftmuonL25reco")*cms.SequencePlaceholder("hltBSoftmuonL25filter")+cms.SequencePlaceholder("hltBSoftmuonL3reco")*cms.SequencePlaceholder("hltBSoftmuonL3filter"))
hltBSoftmuon4jet = cms.Sequence(cms.SequencePlaceholder("prescalerBSoftmuon4jet")+cms.SequencePlaceholder("hltBegin")+cms.SequencePlaceholder("hltBSoftmuonNjetL1seeds")+cms.SequencePlaceholder("hltBCommonL2reco")*cms.SequencePlaceholder("hltBSoftmuon4jetL2filter")+cms.SequencePlaceholder("hltBSoftmuonL25reco")*cms.SequencePlaceholder("hltBSoftmuonL25filter")+cms.SequencePlaceholder("hltBSoftmuonL3reco")*cms.SequencePlaceholder("hltBSoftmuonL3filter"))
hltBSoftmuonHT = cms.Sequence(cms.SequencePlaceholder("prescalerBSoftmuonHT")+cms.SequencePlaceholder("hltBegin")+cms.SequencePlaceholder("hltBSoftmuonHTL1seeds")+cms.SequencePlaceholder("hltBCommonL2reco")*cms.SequencePlaceholder("hltBSoftmuonHTL2filter")+cms.SequencePlaceholder("hltBSoftmuonL25reco")*cms.SequencePlaceholder("hltBSoftmuonL25filter")+cms.SequencePlaceholder("hltBSoftmuonL3reco")*cms.SequencePlaceholder("hltBSoftmuonL3filter"))
