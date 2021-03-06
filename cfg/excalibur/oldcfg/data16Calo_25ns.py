import configtools


def config():
	cfg = configtools.getConfig('data', 2015, 'mm', bunchcrossing='25ns')
	cfg["InputFiles"].set_input(
#		ekppath="/storage/a/cheidecker/cmssw807_calo_freiburg/Zll_DoMuRun2016B-PromptReco-v1/*.root",
#		ekppath="/storage/a/cheidecker/cmssw807_calo_naf/Zll_DoMuRun2016B-PromptReco-v1/*.root",
		ekppath="/storage/a/cheidecker/cmssw807_calo_noPUJetID/Zll_DoMuRun2016B-PromptReco-v2/*.root",
	)	
	cfg['TaggedJets'] = 'ak4CaloJets'
	cfg['RC'] = False #Disable because not available for Calo
	cfg = configtools.expand(cfg, ['nocuts', 'zcuts', 'noalphanoetacuts', 'noalphacuts', 'noetacuts', 'finalcuts'], ['None', 'L1', 'L1L2', 'L1L2L3'])#, 'L1L2L3Res'
	configtools.remove_quantities(cfg, ['jet1btag', 'jet1qgtag', 'jet1ptl1l2l3', 'jet1res', 'jet1rc'])
	
	return cfg
