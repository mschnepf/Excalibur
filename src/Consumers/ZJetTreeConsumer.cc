#include "Consumers/ZJetTreeConsumer.h"
#include "Artus/KappaAnalysis/interface/Producers/ValidElectronsProducer.h"

std::string ZJetTreeConsumer::GetConsumerId() const { return "ZJetTreeConsumer"; }

void ZJetTreeConsumer::Init(ZJetSettings const& settings)
{
    // Add possible quantities for the lambda ntuples consumers

    ////////////////////////
    // General quantities //
    ////////////////////////
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "npu", [](ZJetEvent const& event, ZJetProduct const& product) {
            return event.m_genEventInfo->nPU;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "npumean", [](ZJetEvent const& event, ZJetProduct const& product) {
            return event.m_genEventInfo->nPUMean;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "npumeandata",
        [](ZJetEvent const& event, ZJetProduct const& product) { return product.npumean_data; });
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "hlt", [](ZJetEvent const& event, ZJetProduct const& product) {
            return (!product.m_selectedHltNames.empty());  // check whether any HLT has fired
        });

    ///////
    // Z //
    ///////

    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "zpt",
        [](ZJetEvent const& event, ZJetProduct const& product) { return product.m_z.p4.Pt(); });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "zeta",
        [](ZJetEvent const& event, ZJetProduct const& product) { return product.m_z.p4.Eta(); });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "zphi",
        [](ZJetEvent const& event, ZJetProduct const& product) { return product.m_z.p4.Phi(); });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "zeta",
        [](ZJetEvent const& event, ZJetProduct const& product) { return product.m_z.p4.Eta(); });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "zy", [](ZJetEvent const& event, ZJetProduct const& product) {
            return product.m_z.p4.Rapidity();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "zmass",
        [](ZJetEvent const& event, ZJetProduct const& product) { return product.m_z.p4.mass(); });

    // Gen Z
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genzpt", [](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genZ = product.GetGenZ();
            return (genZ != nullptr) ? genZ->p4.Pt() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genzeta", [](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genZ = product.GetGenZ();
            return (genZ != nullptr) ? genZ->p4.Eta() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genzphi", [](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genZ = product.GetGenZ();
            return (genZ != nullptr) ? genZ->p4.Phi() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genzeta", [](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genZ = product.GetGenZ();
            return (genZ != nullptr) ? genZ->p4.Eta() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genzy", [](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genZ = product.GetGenZ();
            return (genZ != nullptr) ? genZ->p4.Rapidity() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genzmass", [](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genZ = product.GetGenZ();
            return (genZ != nullptr) ? genZ->p4.mass() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "deltarzgenz", [](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genZ = product.GetGenZ();
            return (genZ != nullptr) ? ROOT::Math::VectorUtil::DeltaR(genZ->p4, product.m_z.p4)
                                     : DefaultValues::UndefinedFloat;
        });

    //////////
    // Jets //
    //////////

    // Leading jet
    // basic quantities
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? product.GetValidPrimaryJet(settings, event)->p4.Pt()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1eta", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? product.GetValidPrimaryJet(settings, event)->p4.Eta()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1y", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? product.GetValidPrimaryJet(settings, event)->p4.Rapidity()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1phi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? product.GetValidPrimaryJet(settings, event)->p4.Phi()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1area", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))->area
                       : DefaultValues::UndefinedFloat;
        });
    // PF fractions
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1pf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->photonFraction
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1ef", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->electronFraction
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1chf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->chargedHadronFraction
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1nhf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->neutralHadronFraction
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1mf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->muonFraction
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1hfhf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->hfHadronFraction
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1hfemf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->hfEMFraction
                       : DefaultValues::UndefinedFloat;
        });
    // taggers
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1btag", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->getTag("CombinedSecondaryVertexBJetTags", event.m_jetMetadata)
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1qgtag", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? static_cast<KJet*>(product.GetValidPrimaryJet(settings, event))
                             ->getTag("QGlikelihood", event.m_jetMetadata)
                       : DefaultValues::UndefinedFloat;
        });
    // correction factors
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1l1", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? (product.jetpt_l1 /
                          product.GetValidJet(settings, event, 0, "None")->p4.Pt())
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1rc", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? (product.jetpt_rc /
                          product.GetValidJet(settings, event, 0, "None")->p4.Pt())
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1l2", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? (product.jetpt_l1l2l3 / product.jetpt_l1)
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1res", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? (product.jetpt_l1l2l3res / product.jetpt_l1l2l3)
                       : DefaultValues::UndefinedFloat;
        });

    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1ptraw", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0)
                       ? product.GetValidJet(settings, event, 0, "None")->p4.Pt()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1ptl1", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0) ? product.jetpt_l1
                                                                   : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet1ptl1l2l3", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 0) ? product.jetpt_l1l2l3res
                                                                   : DefaultValues::UndefinedFloat;
        });

    // Second leading jet
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet2pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 1)
                       ? product.GetValidJet(settings, event, 1)->p4.Pt()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet2phi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 1)
                       ? product.GetValidJet(settings, event, 1)->p4.Phi()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet2eta", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 1)
                       ? product.GetValidJet(settings, event, 1)->p4.Eta()
                       : DefaultValues::UndefinedFloat;
        });

    // 3rd leading jet
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet3pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 2)
                       ? product.GetValidJet(settings, event, 2)->p4.Pt()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet3phi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 2)
                       ? product.GetValidJet(settings, event, 2)->p4.Phi()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "jet3eta", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetValidJetCount(settings, event) > 2)
                       ? product.GetValidJet(settings, event, 2)->p4.Eta()
                       : DefaultValues::UndefinedFloat;
        });

    // General jet stuff
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "njets", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetValidJetCount(settings, event);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "njetsinv", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetInvalidJetCount(settings, event);
        });
    
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "njets10", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.CountValidJetsAbovePt(settings, event, 10.0);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "njets30", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.CountValidJetsAbovePt(settings, event, 30.0);
        });
    // Radiation Jets
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "radiationjet1pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetRadiationJetCount(settings, event) > 0)
                       ? product.GetRadiationJet(settings, event, 0)->p4.Pt()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "radiationjet1phi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetRadiationJetCount(settings, event) > 0)
                       ? product.GetRadiationJet(settings, event, 0)->p4.Phi()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "radiationjet1eta", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetRadiationJetCount(settings, event) > 0)
                       ? product.GetRadiationJet(settings, event, 0)->p4.Eta()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "radiationjet1index", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (product.GetRadiationJetCount(settings, event) > 0)
                       ? static_cast<int>(product.GetRadiationJetIndex(settings, event, 0))
                       : DefaultValues::UndefinedInt;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "nradiationjets", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetRadiationJetCount(settings, event);
        });

    // Gen jets
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genjet1pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (event.m_genJets != nullptr && event.m_genJets->size() > 0)
                       ? event.m_genJets->at(0).p4.Pt()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genjet1eta", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (event.m_genJets != nullptr && event.m_genJets->size() > 0)
                       ? event.m_genJets->at(0).p4.Eta()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genjet1phi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (event.m_genJets != nullptr && event.m_genJets->size() > 0)
                       ? event.m_genJets->at(0).p4.Phi()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genjet2pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (event.m_genJets != nullptr && event.m_genJets->size() > 1)
                       ? event.m_genJets->at(1).p4.Pt()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genjet2eta", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (event.m_genJets != nullptr && event.m_genJets->size() > 1)
                       ? event.m_genJets->at(1).p4.Eta()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genjet2phi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return (event.m_genJets != nullptr && event.m_genJets->size() > 1)
                       ? event.m_genJets->at(1).p4.Phi()
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "ngenjets", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetValidJetCount(settings, event, "Gen");
        });
    
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "ngenjets10", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.CountValidJetsAbovePt(settings, event, 10.0, "Gen");
        });

    // Reco jet - gen parton matches
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenparton1pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genParton = product.GetMatchedGenParton(event, settings, 0);
            return (genParton != nullptr) ? genParton->p4.Pt() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "matchedgenparton1flavour", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genParton = product.GetMatchedGenParton(event, settings, 0);
            return (genParton != nullptr) ? static_cast<float>(genParton->pdgId())
                                          : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenparton2pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genParton = product.GetMatchedGenParton(event, settings, 1);
            return (genParton != nullptr) ? genParton->p4.Pt() : DefaultValues::UndefinedFloat;
        });

    // Reco jet - gen jet matches
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenjet1pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KLV* genJet = product.GetMatchedGenJet(event, settings, 0);
            return (genJet != nullptr) ? genJet->p4.Pt() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenjet2pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KLV* genJet = product.GetMatchedGenJet(event, settings, 1);
            return (genJet != nullptr) ? genJet->p4.Pt() : DefaultValues::UndefinedFloat;
        });

    /////////
    // MET //
    /////////

    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "met", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetMet(settings, event)->p4.Pt();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "metphi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetMet(settings, event)->p4.Phi();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "sumet", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetMet(settings, event)->sumEt;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "rawmet", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetMet(settings, event, "None")->p4.Pt();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "rawmetphi", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetMet(settings, event, "None")->p4.Phi();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mpf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetMPF(product.GetMet(settings, event));
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "rawmpf", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.GetMPF(product.GetMet(settings, event, "None"));
        });

    ///////////
    // MUONS //
    ///////////

    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "nmuons", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.m_validMuons.size();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mupluspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() > 0)
                    return (*muon)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mupluseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() > 0)
                    return (*muon)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muplusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() > 0)
                    return (*muon)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muplusiso", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() > 0)
                    return (*muon)->pfIso();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muminuspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() < 0)
                    return (*muon)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muminuseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() < 0)
                    return (*muon)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muminusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() < 0)
                    return (*muon)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muminusiso", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KMuon*>::const_iterator muon = product.m_validMuons.begin();
                 muon != product.m_validMuons.end(); muon++) {
                if ((*muon)->charge() < 0)
                    return (*muon)->pfIso();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1pt", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->p4.Pt()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1phi", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->p4.Phi()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1eta", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->p4.Eta()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1iso", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->pfIso()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1sumchpt", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->sumChargedHadronPt
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1sumnhet", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->sumNeutralHadronEt
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1sumpet", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->sumPhotonEt
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu1sumpupt", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 1 ? product.m_validMuons[0]->sumPUPt
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2pt", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->p4.Pt()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2phi", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->p4.Phi()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2eta", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->p4.Eta()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2iso", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->pfIso()
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2sumchpt", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->sumChargedHadronPt
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2sumnhet", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->sumNeutralHadronEt
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2sumpet", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->sumPhotonEt
                                                    : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "mu2sumpupt", [](event_type const& event, product_type const& product) {
            return product.m_validMuons.size() >= 2 ? product.m_validMuons[1]->sumPUPt
                                                    : DefaultValues::UndefinedFloat;
        });

    // Gen muons
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "ngenmuons", [](ZJetEvent const& event, ZJetProduct const& product) {
            return product.m_genMuons.size();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genmupluspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genMuon = product.m_genMuons.begin();
                 genMuon != product.m_genMuons.end(); genMuon++) {
                if ((*genMuon)->charge() > 0)
                    return (*genMuon)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genmupluseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genMuon = product.m_genMuons.begin();
                 genMuon != product.m_genMuons.end(); genMuon++) {
                if ((*genMuon)->charge() > 0)
                    return (*genMuon)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genmuplusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genMuon = product.m_genMuons.begin();
                 genMuon != product.m_genMuons.end(); genMuon++) {
                if ((*genMuon)->charge() > 0)
                    return (*genMuon)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genmuminuspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genMuon = product.m_genMuons.begin();
                 genMuon != product.m_genMuons.end(); genMuon++) {
                if ((*genMuon)->charge() < 0)
                    return (*genMuon)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genmuminuseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genMuon = product.m_genMuons.begin();
                 genMuon != product.m_genMuons.end(); genMuon++) {
                if ((*genMuon)->charge() < 0)
                    return (*genMuon)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genmuminusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genMuon = product.m_genMuons.begin();
                 genMuon != product.m_genMuons.end(); genMuon++) {
                if ((*genMuon)->charge() < 0)
                    return (*genMuon)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });

    // Reco muon - gen muon matches
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenmuon1pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genMuon = product.GetMatchedGenMuon(event, settings, 0);
            return (genMuon != nullptr) ? genMuon->p4.Pt() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenmuon2pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genMuon = product.GetMatchedGenMuon(event, settings, 1);
            return (genMuon != nullptr) ? genMuon->p4.Pt() : DefaultValues::UndefinedFloat;
        });

    // Invalid muons
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "nmuonsinv", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.m_invalidMuons.size();
        });
    // invalid muon 1
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv1pt", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 1 ? product.m_invalidMuons[0]->p4.Pt()
                                                      : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv1iso", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 1 ? product.m_invalidMuons[0]->pfIso()
                                                      : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv1sumchpt", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 1
                       ? product.m_invalidMuons[0]->sumChargedHadronPt
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv1sumnhet", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 1
                       ? product.m_invalidMuons[0]->sumNeutralHadronEt
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv1sumpet", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 1 ? product.m_invalidMuons[0]->sumPhotonEt
                                                      : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv1sumpupt", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 1 ? product.m_invalidMuons[0]->sumPUPt
                                                      : DefaultValues::UndefinedFloat;
        });
    // invalid muon 2
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv2pt", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 2 ? product.m_invalidMuons[1]->p4.Pt()
                                                      : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv2iso", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 2 ? product.m_invalidMuons[1]->pfIso()
                                                      : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv2sumchpt", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 2
                       ? product.m_invalidMuons[1]->sumChargedHadronPt
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv2sumnhet", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 2
                       ? product.m_invalidMuons[1]->sumNeutralHadronEt
                       : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv2sumpet", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 2 ? product.m_invalidMuons[1]->sumPhotonEt
                                                      : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "muinv2sumpupt", [](event_type const& event, product_type const& product) {
            return product.m_invalidMuons.size() >= 2 ? product.m_invalidMuons[1]->sumPUPt
                                                      : DefaultValues::UndefinedFloat;
        });

    ///////////////
    // ELECTRONS //
    ///////////////
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "nelectrons", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            return product.m_validElectrons.size();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1pt", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[0]->p4.Pt()
                                                        : DefaultValues::UndefinedFloat;
        });
    // Basic kinematics
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1phi", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[0]->p4.Phi()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1eta", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[0]->p4.Eta()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2pt", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 2 ? product.m_validElectrons[1]->p4.Pt()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2phi", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 2 ? product.m_validElectrons[1]->p4.Phi()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2eta", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 2 ? product.m_validElectrons[1]->p4.Eta()
                                                        : DefaultValues::UndefinedFloat;
        });
    // IDs
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1looseid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[0]->idLoose()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1mediumid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[0]->idMedium()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1tightid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[0]->idTight()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1vetoid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[0]->idVeto()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1looseid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsLooseVbtf95Electron(
                product.m_validElectrons[0], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1mediumid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsMediumVbtf95Electron(
                product.m_validElectrons[0], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1tightid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsTightVbtf95Electron(
                product.m_validElectrons[0], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1tightid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsTightVbtf95Electron(
                product.m_validElectrons[0], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1mvatrig", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsMVATrigElectron(product.m_validElectrons[0],
                                                                        event.m_electronMetadata);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e1mvanontrig", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsMVANonTrigElectron(
                product.m_validElectrons[0], event.m_electronMetadata);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2looseid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsLooseVbtf95Electron(
                product.m_validElectrons[1], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2mediumid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsMediumVbtf95Electron(
                product.m_validElectrons[1], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2tightid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsTightVbtf95Electron(
                product.m_validElectrons[1], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2tightid95", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsTightVbtf95Electron(
                product.m_validElectrons[1], event, product);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2mvatrig", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsMVATrigElectron(product.m_validElectrons[1],
                                                                        event.m_electronMetadata);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2mvanontrig", [](event_type const& event, product_type const& product) {
            return ValidElectronsProducer<ZJetTypes>::IsMVANonTrigElectron(
                product.m_validElectrons[1], event.m_electronMetadata);
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2looseid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[1]->idLoose()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2mediumid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[1]->idMedium()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2tightid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[1]->idTight()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "e2vetoid", [](event_type const& event, product_type const& product) {
            return product.m_validElectrons.size() >= 1 ? product.m_validElectrons[1]->idVeto()
                                                        : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "epluspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() > 0)
                    return (*electron)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "epluseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() > 0)
                    return (*electron)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "eplusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() > 0)
                    return (*electron)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "eplusiso", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() > 0)
                    return (*electron)->pfIso();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "eminuspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() < 0)
                    return (*electron)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "eminuseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() < 0)
                    return (*electron)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "eminusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() < 0)
                    return (*electron)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "eminusiso", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KElectron*>::const_iterator electron =
                     product.m_validElectrons.begin();
                 electron != product.m_validElectrons.end(); electron++) {
                if ((*electron)->charge() < 0)
                    return (*electron)->pfIso();
            }
            return DefaultValues::UndefinedFloat;
        });
    // gen electrons
    LambdaNtupleConsumer<ZJetTypes>::AddIntQuantity(
        "ngenelectrons", [](ZJetEvent const& event, ZJetProduct const& product) {
            return product.m_genElectrons.size();
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genepluspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genElectron =
                     product.m_genElectrons.begin();
                 genElectron != product.m_genElectrons.end(); genElectron++) {
                if ((*genElectron)->charge() > 0)
                    return (*genElectron)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "genepluseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genElectron =
                     product.m_genElectrons.begin();
                 genElectron != product.m_genElectrons.end(); genElectron++) {
                if ((*genElectron)->charge() > 0)
                    return (*genElectron)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "geneplusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genElectron =
                     product.m_genElectrons.begin();
                 genElectron != product.m_genElectrons.end(); genElectron++) {
                if ((*genElectron)->charge() > 0)
                    return (*genElectron)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "geneminuspt", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genElectron =
                     product.m_genElectrons.begin();
                 genElectron != product.m_genElectrons.end(); genElectron++) {
                if ((*genElectron)->charge() < 0)
                    return (*genElectron)->p4.Pt();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "geneminuseta", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genElectron =
                     product.m_genElectrons.begin();
                 genElectron != product.m_genElectrons.end(); genElectron++) {
                if ((*genElectron)->charge() < 0)
                    return (*genElectron)->p4.Eta();
            }
            return DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "geneminusphi", [](ZJetEvent const& event, ZJetProduct const& product) -> float {
            for (std::vector<KGenParticle*>::const_iterator genElectron =
                     product.m_genElectrons.begin();
                 genElectron != product.m_genElectrons.end(); genElectron++) {
                if ((*genElectron)->charge() < 0)
                    return (*genElectron)->p4.Phi();
            }
            return DefaultValues::UndefinedFloat;
        });
    // Reco muon - gen muon matches
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenelectron1pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genElectron = product.GetMatchedGenElectron(event, settings, 0);
            return (genElectron != nullptr) ? genElectron->p4.Pt() : DefaultValues::UndefinedFloat;
        });
    LambdaNtupleConsumer<ZJetTypes>::AddFloatQuantity(
        "matchedgenelectron2pt", [settings](ZJetEvent const& event, ZJetProduct const& product) {
            KGenParticle* genElectron = product.GetMatchedGenElectron(event, settings, 1);
            return (genElectron != nullptr) ? genElectron->p4.Pt() : DefaultValues::UndefinedFloat;
        });

    // Needs to be called at the end
    KappaLambdaNtupleConsumer::Init(settings);
}
