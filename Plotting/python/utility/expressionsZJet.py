
# -*- coding: utf-8 -*-

import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

from Artus.HarryPlotter.utility.expressions import ExpressionsDict

"""
	This module contains a dictionary for expressions.
"""
class ExpressionsDictZJet(ExpressionsDict):
    def __init__(self):
        super(ExpressionsDictZJet, self).__init__()
        self.expressions_dict.update({
            'alpha': '(jet2pt/zpt)',
            'genalpha': '(genjet2pt/genzpt)',
            #'A': '(zpt-jet1pt)/(zpt+jet1pt)',
            'B': '1-mpf',
            'absjet1eta': 'abs(jet1eta)',
            #'ystar': 'abs(zy-jet1y)/2',
            #'yboost': 'abs(zy+jet1y)/2',
            'jetavept': '(jet1pt+jet2pt)/2',
            'jetystar':  'abs(jet1y-jet2y)/2',
            'jetyboost': 'abs(jet1y+jet2y)/2',
            #'genystar': 'abs(genzy-genjet1y)/2',
            #'genyboost': 'abs(genzy+genjet1y)/2',
            'ptbalance': '(jet1pt/zpt)',
            'genptbalance': '(genjet1pt/genzpt)',
            'backtoback' : '(abs(delta_phi(z,jet1)-TMath::Pi()))',
            'trueresponse': '(jet1pt/genjet1pt)',
            'deltaphizjet1' : '(abs(abs(abs(zphi-jet1phi)-TMath::Pi())-TMath::Pi()))',
            'gendeltaphizjet1' : '(abs(abs(abs(genzphi-genjet1phi)-TMath::Pi())-TMath::Pi()))',
            'deltaphizeminus' : '(abs(abs(abs(zphi-eminusphi)-TMath::Pi())-TMath::Pi()))',
            'deltaphizmuminus' : '(abs(abs(abs(zphi-muminusphi)-TMath::Pi())-TMath::Pi()))',
            'deltaphizmuplus' : '(abs(abs(abs(zphi-muplusphi)-TMath::Pi())-TMath::Pi()))',
            'deltaphizeplus' : '(abs(abs(abs(zphi-eplusphi)-TMath::Pi())-TMath::Pi()))',
            'deltaphieminuseplus' : '(abs(abs(abs(eminusphi-eplusphi)-TMath::Pi())-TMath::Pi()))',
            'deltaphijet1jet2' : '(abs(abs(abs(jet1phi-jet2phi)-TMath::Pi())-TMath::Pi()))',
            'gendeltaphijet1jet2' : '(abs(abs(abs(genjet1phi-genjet2phi)-TMath::Pi())-TMath::Pi()))',
            'deltaetajet1jet2' : '(abs(jet1eta-jet2eta))',
            'deltarjet1jet2': 'sqrt((abs(abs(abs(jet1phi-jet2phi)-TMath::Pi())-TMath::Pi()))*(abs(abs(abs(jet1phi-jet2phi)-TMath::Pi())-TMath::Pi()))+(abs(jet1eta-jet2eta)**2))',
            'deltarjet1mu1': 'sqrt((abs(abs(abs(jet1phi-mu1phi)-TMath::Pi())-TMath::Pi())**2)+(abs(jet1eta-mu1eta)**2))',
            'deltaphizmet' : '(abs(abs(abs(zphi-metphi)-TMath::Pi())-TMath::Pi()))',
            'jet1abseta' : 'abs(jet1eta)',
            'jet1match': 'sqrt((abs(abs(abs(jet1phi-genjet1phi)-TMath::Pi())-TMath::Pi()))**2+(jet1y-genjet1y)**2)',
            'sortedflavour' : (
                'matchedgenparton1flavour*(matchedgenparton1flavour<20 && matchedgenparton1flavour>(-20))'  # quarks: ok
                +'+6*(matchedgenparton1flavour==21)'  # gluons: 21 -> 6
                +'+7*(matchedgenparton1flavour==-999)'  # undef ->7
            ),
            'sortedabsflavour' : (
                'abs(matchedgenparton1flavour)*(abs(matchedgenparton1flavour)<20)'  # quarks: ok
                +'+6*(abs(matchedgenparton1flavour)==21)'  # gluons: 21 -> 6
                +'+7*(abs(matchedgenparton1flavour)==999)'  # undef ->7
            ),
            'deltarjet1radiationjet1': 'sqrt((abs(abs(abs(jet1phi-radiationjet1phi)-TMath::Pi())-TMath::Pi()))*(abs(abs(abs(jet1phi-radiationjet1phi)-TMath::Pi())-TMath::Pi()))+(abs(jet1eta-radiationjet1eta)**2))',
        })
        self.expressions_regex += [
            (r"delta_r\(([-\w]+),([-\w]+)\)",r"sqrt((delta_phi(\1,\2)**2)+(delta_eta(\1,\2)**2))"),
            (r"delta_phi\(([-\w]+),([-\w]+)\)", lambda matchobj: r"abs(abs(abs(" + (matchobj.group(1)[1:]+"phi-TMath::Pi()" if matchobj.group(1)[0] == "-" else matchobj.group(1)+"phi" ) + "-" + (matchobj.group(2)[1:]+"phi+TMath::Pi()" if matchobj.group(2)[0] == "-" else matchobj.group(2)+"phi" ) + ")-TMath::Pi())-TMath::Pi())"),
            (r"delta_eta\(([-\w]+),([-\w]+)\)",r"abs(\1eta-\2eta)"),
        ]
