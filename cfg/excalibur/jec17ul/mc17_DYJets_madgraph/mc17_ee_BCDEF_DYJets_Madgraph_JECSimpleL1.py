
import configtools
import os
import sys

# -- import common information
sys.path.append(os.path.dirname(__file__))
from common import JEC_BASE, JEC_VERSION, JER, SE_PATH_PREFIXES

RUNS=['B', 'C', 'D', 'E', 'F']
CH='ee'
#JEC='{0}_{1}_SimpleL1'.format(JEC_BASE, JEC_VERSION)
JEC='{0}_{1}'.format(JEC_BASE, JEC_VERSION)


def config():
    cfg = configtools.getConfig('mc', 2017, CH, JEC=JEC, JER=JER)
    cfg["InputFiles"].set_input(
        path="{}/mhorzela/Skimming/ZJet_DYJetsToLL_Summer19-madgraphMLM_realistic_v6-v3/*.root".format(SE_PATH_PREFIXES["xrootd_gridka_nrg"]),
    )
    cfg['JsonFiles'] = [os.path.join(configtools.getPath(), 'data/json/Collisions17/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt')]

    cfg['Pipelines']['default']['Quantities'] += ['puWeight{}'.format(runperiod) for runperiod in ['B', 'C', 'D', 'E', 'F']]
    # cfg['Pipelines']['default']['Quantities'] += ['genWeight_{}'.format(lheWeightName) for lheWeightName in ['nominal', 'isrDefup', 'isrDefdown', 'fsrDefup', 'fsrDefdown']]
    cfg['Pipelines']['default']['Quantities'] += ['jet1chf', 'jet1nhf', 'jet1ef', 'jet1mf', 'jet1hfhf', 'jet1hfemf', 'jet1pf']
    cfg = configtools.expand(cfg, ['basiccuts','finalcuts'], ['None', 'L1', 'L1L2L3'])

    cfg['PileupWeightFile'] = os.path.join(configtools.getPath() , 'data/pileup/mc_weights/mc17ul_DYJets_madgraph_data_15May18/PUWeights_' + ''.join(['B', 'C', 'D', 'E', 'F']) + '_15May2018_DYJetsToLL_madgraphMLM.root')
    cfg['NumberGeneratedEvents'] = 101077576
    cfg['GeneratorWeight'] = 1.0
    cfg['CrossSection'] = 6077.22  # from XSDB: https://cms-gen-dev.cern.ch/xsdb/?searchQuery=DAS=DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8

    cfg['CutBackToBack'] = 0.44

    cfg['VertexSummary'] = 'offlinePrimaryVerticesSummary'

    cfg['ApplyElectronVID'] = True
    cfg['ElectronVIDName'] = "Fall17-94X-V2"
    cfg['ElectronVIDType'] = "cutbased"
    cfg['ElectronVIDWorkingPoint'] = "tight"

    cfg['CutJetID'] = 'tightlepveto'  # choose event-based JetID selection
    cfg['CutJetIDVersion'] = '2017UL'  # for event-based JetID
    cfg['CutJetIDFirstNJets'] = 2

    cfg['Processors'] += ['producer:ZJetPUWeightProducer']
    cfg['ZJetPUWeightFiles'] = [os.path.join(configtools.getPath() ,'data/pileup/mc_weights/mc17_DYJets_madgraph/PUWeights_{}_17Nov2017_DY1JetsToLL_Fall17-madgraphMLM_realistic_v10-v1.root'.format(runperiod)) for runperiod in ['B', 'C', 'D', 'E', 'F']]
    cfg['ZJetPUWeightSuffixes'] = ['{}'.format(runperiod) for runperiod in ['B', 'C', 'D', 'E', 'F']]

    # cfg['ZJetGenWeightNames'] = ['nominal', 'isrDefup', 'isrDefdown', 'fsrDefup', 'fsrDefdown']

    return cfg
