#pragma once

#include "Excalibur/Compile/interface/ZJetTypes.h"

#include <random>

/*
    This producers calculates NPU ("mu", the expected pileup) in data per run and lumisection from a
   txt file in a specific format.
*/

class NPUProducer : public ZJetProducerBase
{
  public:
    std::string GetProducerId() const;

    NPUProducer() : ZJetProducerBase(), lastrun(0), lastls(0) {}

    void Init(ZJetSettings const& settings);

    void Produce(ZJetEvent const& event, ZJetProduct& product, ZJetSettings const& settings) const;

  private:
    std::map<unsigned long, std::map<unsigned long, float>> m_pumean;
    std::map<unsigned long, std::map<unsigned long, float>> m_pumeanrms;
    mutable unsigned long lastrun;
    mutable unsigned long lastls;
    bool m_smearing = false;
    mutable std::mt19937 m_randomNumberGenerator;
};
