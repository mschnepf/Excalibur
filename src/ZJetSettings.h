#pragma once

#include "Artus/KappaAnalysis/interface/KappaSettings.h"

class ZJetSettings : public KappaSettings
{
  public:
	IMPL_SETTING_DEFAULT(std::string, JetAlgorithm, "")

	IMPL_SETTING(bool, MuonID2011)
	IMPL_SETTING(float, MuonEtaMax)
	IMPL_SETTING(float, MuonPtMin)

	IMPL_SETTING(float, ZMass)
	IMPL_SETTING(float, ZMassRange)

	IMPL_SETTING(bool, VetoPileupJets)

	IMPL_SETTING(float, ZPtMin)

	IMPL_SETTING(float, JetPtMin)
	IMPL_SETTING(float, JetEtaMax)
	IMPL_SETTING(float, AlphaMax)
	IMPL_SETTING(float, DeltaPhiMax)
};