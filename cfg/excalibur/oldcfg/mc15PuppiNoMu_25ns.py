import configtools
import mc15Puppi_25ns


def config():
	cfg = mc15Puppi_25ns.config()
	cfg['TaggedJets'] = 'ak4PFJetsPuppiNoMu'
	cfg['Met'] = 'metPuppiNoMu'
	#cfg['MetAddMuons'] = True
	return cfg
