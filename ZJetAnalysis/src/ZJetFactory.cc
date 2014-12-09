#include "ZJet/ZJetAnalysis/interface/ZJetFactory.h"

// producers
//#include "ZJet/ZJetAnalysis/interface/Producers/ZJetValidJetsProducer.h"

// filters
//#include "ZJet/ZJetAnalysis/interface/Filters/MuonFilter.h"

// consumers
#include "ZJet/ZJetAnalysis/interface/ZJetNtupleConsumer.h"



ProducerBaseUntemplated * ZJetFactory::createProducer ( std::string const& id )
{
	//if(id == ValidMuonProducer().GetProducerId())
	//	return new ValidMuonProducer();
	//else
		return KappaFactory::createProducer( id );	
}

FilterBaseUntemplated * ZJetFactory::createFilter ( std::string const& id )
{
	//if(id == MuonFilter().GetFilterId())
	//	return new MuonFilter();
	//else
		return KappaFactory::createFilter( id );
}

ConsumerBaseUntemplated * ZJetFactory::createConsumer ( std::string const& id )
{
	if(id == ZJetLambdaNtupleConsumer().GetConsumerId())
		return new ZJetLambdaNtupleConsumer();
	else
		return KappaFactory::createConsumer( id );
}
