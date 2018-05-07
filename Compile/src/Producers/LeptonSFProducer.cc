#include "Excalibur/Compile/interface/Producers/LeptonSFProducer.h"

#include <boost/algorithm/string.hpp>
#include "TH2.h"
#include "TGraphAsymmErrors.h"

/*
taken from https://twiki.cern.ch/twiki/bin/view/Main/EGammaScaleFactors2012

This producer contains some un-intuitive programming constructs, which are
necessary to account for the different formats of the ROOT files containing
the scale factors (different axes, binning in absolute eta)

needed tags:
- LeptonSFVariation      Vary SF by error. Choices: up, down, None


maybe also relevant:
https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentification (SF files for MVA
ID)
https://twiki.cern.ch/twiki/bin/view/CMSPublic/DiLeptonAnalysis#Efficiencies_and_Scale_Factors
https://twiki.cern.ch/twiki/bin/viewauth/CMS/DileptonTriggerResults
http://lovedeep.web.cern.ch/lovedeep/WORK13/TnPRun2012ReReco_2013Oct28/effiPlots.html
*/

//////////////
// LeptonSF //
//////////////

std::string LeptonSFProducer::GetProducerId() const { return "LeptonSFProducer"; }

void LeptonSFProducer::Init(ZJetSettings const& settings)
{
   
}

void LeptonSFProducer::Produce(ZJetEvent const& event,
                                 ZJetProduct& product,
                                 ZJetSettings const& settings) const
{
    
}

////////////////
// LeptonIDSF //
////////////////

std::string LeptonIDSFProducer::GetProducerId() const { return "LeptonIDSFProducer"; }

void LeptonIDSFProducer::Init(ZJetSettings const& settings)
{
    m_sffile = settings.GetLeptonIDSFRootfile();
    std::string histoname;  // histoname depending on id
    double error_multiplier = 0.;
    if (settings.GetLeptonSFVariation() == "up") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor UP one sigma";
        error_multiplier = 1.;
    } else if (settings.GetLeptonSFVariation() == "down") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor DOWN one sigma";
        error_multiplier = -1.;
    }
    if(settings.GetChannel() == "mm"){
        if (settings.GetInputIsData()){
            if(settings.GetMuonID() == "tight"){
                //histoname = "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/efficienciesDATA/abseta_pt_DATA";
                histoname = "MC_NUM_TightID_DEN_genTracks_PAR_eta/efficienciesDATA/histo_eta_DATA";
            }
            if(settings.GetMuonID() == "medium"){
                //histoname = "MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/efficienciesDATA/abseta_pt_DATA";
                histoname = "MC_NUM_MediumID_DEN_genTracks_PAR_eta/efficienciesDATA/histo_eta_DATA";
            }
            if(settings.GetMuonID() == "loose"){
                //histoname = "MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/efficienciesDATA/abseta_pt_DATA";
                histoname = "MC_NUM_LooseID_DEN_genTracks_PAR_eta/efficienciesDATA/histo_eta_DATA";
            }
        }
        else{
            if(settings.GetMuonID() == "tight"){
                //histoname = "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/efficienciesMC/abseta_pt_MC";
                //histoname = "MC_NUM_TightID_DEN_genTracks_PAR_pt/efficienciesMC/histo_pt_MC";
                histoname = "MC_NUM_TightID_DEN_genTracks_PAR_eta/efficienciesMC/histo_eta_MC";
            }
            if(settings.GetMuonID() == "medium"){
                //histoname = "MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/efficienciesMC/abseta_pt_MC";
                histoname = "MC_NUM_MediumID_DEN_genTracks_PAR_eta/efficienciesMC/histo_eta_MC";
            }
            if(settings.GetMuonID() == "loose"){
                //histoname = "MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/efficienciesMC/abseta_pt_MC";
                histoname = "MC_NUM_LooseID_DEN_genTracks_PAR_eta/efficienciesMC/histo_eta_MC";
            }
        }
    }
    else{
        LOG(ERROR) << "LeptonIsoSFProducer not implemented for this channel";
    }
    
    m_etabins = &m_xbins;
    m_ptbins = &m_ybins;
    //m_absoluteeta = true;
    //m_reversed_axes = true;
    
    // Get file
    LOG(INFO) << "Loading lepton scale factors for ID " << m_id << ": File " << m_sffile
              << ", Histogram " << histoname;
    TFile file(m_sffile.c_str(), "READONLY");
    TH2F* sfhisto = (TH2F*)file.Get(histoname.c_str());

   // Get the pT and eta bin borders
    for (int ix = 0; ix <= sfhisto->GetNbinsX(); ++ix)
        m_xbins.emplace_back(2 * sfhisto->GetXaxis()->GetBinCenter(ix) -
                             sfhisto->GetXaxis()->GetBinLowEdge(ix));
    for (int iy = 0; iy <= sfhisto->GetNbinsY(); ++iy)
        m_ybins.emplace_back(2 * sfhisto->GetYaxis()->GetBinCenter(iy) -
                             sfhisto->GetYaxis()->GetBinLowEdge(iy));
    // Fill the m_sf array with the values from the root histo
    for (int ix = 1; ix <= sfhisto->GetNbinsX(); ix++) {
        for (int iy = 1; iy <= sfhisto->GetNbinsY(); iy++) {
            m_sf[0][ix - 1][iy - 1] = static_cast<float>(
                sfhisto->GetBinContent(ix, iy) + error_multiplier * sfhisto->GetBinError(ix, iy));
        }
    }
    delete sfhisto;
    file.Close();

}

void LeptonIDSFProducer::Produce(ZJetEvent const& event,
                                 ZJetProduct& product,
                                 ZJetSettings const& settings) const
{
    if(product.m_zValid){
        product.m_weights["leptonIDSFWeight"] =
                //some old SF files already include the inverse, make sure you use them the right way!
                //GetScaleFactor(0, *product.m_zLeptons.first) * GetScaleFactor(0, *product.m_zLeptons.second);
                1/GetScaleFactor(0, *product.m_zLeptons.first) * 1/GetScaleFactor(0, *product.m_zLeptons.second);
        product.m_weights["mu1IDSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.first);
        product.m_weights["mu2IDSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.second);
    }
    else
        product.m_weights["leptonIDSFWeight"] = 0;
}

/////////////////
// LeptonIsoSF //
/////////////////

std::string LeptonIsoSFProducer::GetProducerId() const { return "LeptonIsoSFProducer"; }

void LeptonIsoSFProducer::Init(ZJetSettings const& settings)
{
    m_sffile = settings.GetLeptonIsoSFRootfile();
    std::string histoname;  // histoname depending on id
    double error_multiplier = 0.;
    if (settings.GetLeptonSFVariation() == "up") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor UP one sigma";
        error_multiplier = 1.;
    } else if (settings.GetLeptonSFVariation() == "down") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor DOWN one sigma";
        error_multiplier = -1.;
    }
    
    if(settings.GetChannel() == "mm"){
        if(settings.GetInputIsData()){
            if(settings.GetMuonID() == "tight"){
                if(settings.GetMuonIso() == "tight_2016")
                    //histoname = "TightISO_TightID_pt_eta/efficienciesDATA/abseta_pt_DATA";
                    histoname = "TightISO_TightID_eta/efficienciesDATA/histo_eta_DATA";
                else if (settings.GetMuonIso() == "loose_2016")
                    //histoname = "LooseISO_TightID_pt_eta/efficienciesDATA/abseta_pt_DATA";
                    histoname = "LooseISO_TightID_eta/efficienciesDATA/histo_eta_DATA";
                else
                    LOG(ERROR) << "No Efficiencies for this Isolation";
                }
            if(settings.GetMuonID() == "medium"){
                if(settings.GetMuonIso() == "tight_2016")
                    //histoname = "TightISO_MediumID_pt_eta/efficienciesDATA/abseta_pt_DATA";
                    histoname = "TightISO_MediumID_eta/efficienciesDATA/histo_eta_DATA";
                else if (settings.GetMuonIso() == "loose_2016")
                    //histoname = "LooseISO_MediumID_pt_eta/efficienciesDATA/abseta_pt_DATA";
                    histoname = "LooseISO_MediumID_eta/efficienciesDATA/histo_eta_DATA";
                else
                    LOG(ERROR) << "No Efficiencies for this Isolation";
                }
            if(settings.GetMuonID() == "loose"){
                if(settings.GetMuonIso() == "tight_2016")
                    LOG(ERROR) << "No Efficiencies for loose ID and tight Iso";
                else if (settings.GetMuonIso() == "loose_2016")
                    //histoname = "LooseISO_LooseID_pt_eta/efficienciesDATA/abseta_pt_DATA";
                    histoname = "LooseISO_LooseID_eta/efficienciesDATA/histo_eta_DATA";
                else
                    LOG(ERROR) << "No Efficiencies for this Isolation";
                }
            }
        else{
            if(settings.GetMuonID() == "tight"){
                if(settings.GetMuonIso() == "tight_2016")
                    //histoname = "TightISO_TightID_pt_eta/efficienciesMC/abseta_pt_MC";
                    histoname = "TightISO_TightID_eta/efficienciesMC/histo_eta_MC";
                else if (settings.GetMuonIso() == "loose_2016")
                    //histoname = "LooseISO_TightID_pt_eta/efficienciesMC/abseta_pt_MC";
                    histoname = "LooseISO_TightID_eta/efficienciesMC/histo_eta_MC";
                else
                    LOG(ERROR) << "No Efficiencies for this Isolation";
                }
            if(settings.GetMuonID() == "medium"){
                if(settings.GetMuonIso() == "tight_2016")
                    //histoname = "TightISO_MediumID_pt_eta/efficienciesMC/abseta_pt_MC";
                    histoname = "TightISO_MediumID_eta/efficienciesMC/histo_eta_MC";
                else if (settings.GetMuonIso() == "loose_2016")
                    //histoname = "LooseISO_MediumID_pt_eta/efficienciesMC/abseta_pt_MC";
                    histoname = "LooseISO_MediumID_eta/efficienciesMC/histo_eta_MC";
                else
                    LOG(ERROR) << "No Efficiencies for this Isolation";
                }
            if(settings.GetMuonID() == "loose"){
                if(settings.GetMuonIso() == "tight_2016")
                    LOG(ERROR) << "No Efficiencies for loose ID and tight Iso";
                else if (settings.GetMuonIso() == "loose_2016")
                    //histoname = "LooseISO_LooseID_pt_eta/efficienciesMC/abseta_pt_MC";
                    histoname = "LooseISO_LooseID_eta/efficienciesMC/histo_eta_MC";
                else
                    LOG(ERROR) << "No Efficiencies for this Isolation";
                }
            }
        }
    else{
        LOG(ERROR) << "LeptonIsoSFProducer not implemented for this channel";
    }

    m_etabins = &m_xbins;
    m_ptbins = &m_ybins;
    //m_absoluteeta = true;
    //m_reversed_axes = true;
    
    // Get file
    LOG(INFO) << "Loading lepton scale factors for Isolation " << m_id << ": File " << m_sffile
              << ", Histogram " << histoname;
    TFile file(m_sffile.c_str(), "READONLY");
    TH2F* sfhisto = (TH2F*)file.Get(histoname.c_str());

    // Get the pT and eta bin borders
    for (int ix = 0; ix <= sfhisto->GetNbinsX(); ++ix)
        m_xbins.emplace_back(2 * sfhisto->GetXaxis()->GetBinCenter(ix) -
                             sfhisto->GetXaxis()->GetBinLowEdge(ix));
    for (int iy = 0; iy <= sfhisto->GetNbinsY(); ++iy)
        m_ybins.emplace_back(2 * sfhisto->GetYaxis()->GetBinCenter(iy) -
                             sfhisto->GetYaxis()->GetBinLowEdge(iy));
    // Fill the m_sf array with the values from the root histo
    for (int ix = 1; ix <= sfhisto->GetNbinsX(); ix++) {
        for (int iy = 1; iy <= sfhisto->GetNbinsY(); iy++) {
            m_sf[0][ix - 1][iy - 1] = static_cast<float>(
                sfhisto->GetBinContent(ix, iy) + error_multiplier * sfhisto->GetBinError(ix, iy));
        }
    }
    delete sfhisto;
    file.Close();
}

void LeptonIsoSFProducer::Produce(ZJetEvent const& event,
                                 ZJetProduct& product,
                                 ZJetSettings const& settings) const
{
    if(product.m_zValid){
        product.m_weights["mu1IsoSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.first);
        product.m_weights["mu2IsoSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.second);
        product.m_weights["leptonIsoSFWeight"] =
                //some old SF files already include the inverse, make sure you use them the right way!
                //GetScaleFactor(0, *product.m_zLeptons.first) * GetScaleFactor(0, *product.m_zLeptons.second);
                1/GetScaleFactor(0, *product.m_zLeptons.first) * 1/GetScaleFactor(0, *product.m_zLeptons.second);
    }
    else
        product.m_weights["leptonIsoSFWeight"] = 0;
}

//////////////////////
// LeptonTrackingSF //
//////////////////////

std::string LeptonTrackingSFProducer::GetProducerId() const { return "LeptonTrackingSFProducer"; }

void LeptonTrackingSFProducer::Init(ZJetSettings const& settings)
{
    m_sffile = settings.GetLeptonTrackingSFRootfile();
    //m_id = settings.GetMuonID();
    //m_iso = settings.GetMuonIso();
    std::string histoname;   // histoname depending on id
    double error_multiplier = 0.;
    if (settings.GetLeptonSFVariation() == "up") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor UP one sigma";
        error_multiplier = 1.;
    } else if (settings.GetLeptonSFVariation() == "down") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor DOWN one sigma";
        error_multiplier = -1.;
    }
    
    if(settings.GetChannel() == "mm"){
        histoname = "ratio_eff_eta3_dr030e030_corr";
    }
    else{
        LOG(ERROR) << "LeptonTrackingSFProducer not implemented for this channel";
    }
    
    m_etabins = &m_xbins;
    m_ptbins = &m_ybins;
    //m_absoluteeta = true;
    //m_reversed_axes = true;

    // Get file
    LOG(INFO) << "Loading lepton scale factors for Tracking: File " << m_sffile
              << ", Histogram " << histoname;
    TFile file(m_sffile.c_str(), "READONLY");
    //TH2F* sfhisto = (TH2F*)file.Get(histoname.c_str());
    TGraphAsymmErrors *sfhisto = (TGraphAsymmErrors*)file.Get(histoname.c_str());
    //TGraphErrors *sfhisto = (TGraphErrors*)gDirectory->Get("ratio_eff_aeta_dr030e030_corr");

    // Get the pT and eta bin borders
    m_xbins.emplace_back(sfhisto->GetX()[0] - sfhisto->GetErrorXlow(0));
    for (int ix = 0; ix < sfhisto->GetN(); ++ix){
        m_xbins.emplace_back(sfhisto->GetX()[ix] + sfhisto->GetErrorXhigh(ix));
    }
    m_ybins.emplace_back(0);
    m_ybins.emplace_back(10000);
    
    // Fill the m_sf array with the values from the root histos
    //LOG(INFO) << "x first border no." << 0 << ": "<< m_xbins[0];
    for (int ix = 1; ix <= sfhisto->GetN(); ix++) {
        m_sf[0][ix-1][0] = static_cast<float>(sfhisto->GetY()[ix-1] + error_multiplier * sfhisto->GetErrorY(ix-1));
        //LOG(INFO) << "value: " << m_sf[0][ix-1][0];
        //LOG(INFO) << "x upper border no."<< ix << ": "<< m_xbins[ix];
    }
    delete sfhisto;
    file.Close();
}

void LeptonTrackingSFProducer::Produce(ZJetEvent const& event,
                                 ZJetProduct& product,
                                 ZJetSettings const& settings) const
{
    //LOG(INFO) << "SF: " << m_sf[0][0][0];
    if(product.m_zValid){
        product.m_weights["mu1TrackingSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.first);
        product.m_weights["mu2TrackingSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.second);
        product.m_weights["leptonTrackingSFWeight"] =
                //some old SF files already include the inverse, make sure you use them the right way!
                //GetScaleFactor(0, *product.m_zLeptons.first) * GetScaleFactor(0, *product.m_zLeptons.second);
                1/GetScaleFactor(0, *product.m_zLeptons.first) * 1/GetScaleFactor(0, *product.m_zLeptons.second);
    }
    else
        product.m_weights["leptonTrackingSFWeight"] = 0;
}

/////////////////////
// LeptonTriggerSF //
/////////////////////

std::string LeptonTriggerSFProducer::GetProducerId() const { return "LeptonTriggerSFProducer"; }

void LeptonTriggerSFProducer::Init(ZJetSettings const& settings)
{
    m_sffile = settings.GetLeptonTriggerSFRootfile();
    std::string histoname;
    double error_multiplier = 0.;
    if (settings.GetLeptonTriggerSFVariation() == "up") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor UP one sigma";
        error_multiplier = 1.;
    } 
    else if (settings.GetLeptonTriggerSFVariation() == "down") {
        LOG(WARNING) << "LeptonSFProducer: varying scale factor DOWN one sigma";
        error_multiplier = -1.;
    }
    
    if(settings.GetChannel() == "mm"){
        if(settings.GetInputIsData()){
            //histoname = "IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesDATA/abseta_pt_DATA";
            histoname = "IsoMu24_OR_IsoTkMu24_EtaBins/efficienciesDATA/histo_eta_DATA";
        }
        else{
            //histoname = "IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/abseta_pt_MC";
            histoname = "IsoMu24_OR_IsoTkMu24_EtaBins/efficienciesMC/histo_eta_MC";
        }
    }
    else{
        LOG(ERROR) << "LeptonTriggerSFProducer not implemented for this channel";
    }
    m_etabins = &m_xbins;
    m_ptbins = &m_ybins;
    //m_absoluteeta = true;
    //m_reversed_axes = true;

    // Get file
    //runs = settings.GetTriggerSFRuns();
    //std::vector<TH2F*> sfhistos;
    //int runcount = runs.size();
    //if(runs.size()>0){
        //for (int i = 0; i < runcount; i++)
            //sfhistos.push_back((TH2F*) file.Get(std::to_string(runs.at(i)).c_str()));
    //}
    //else{
        //sfhistos.push_back((TH2F*) file.Get());
        //sfhistos.push_back((TH2F*) file.Get("histoSF")); //check the TriggerSF histo names!
        //runcount = 1;
    //}
    
    // Get file
    LOG(INFO) << "Loading lepton scale factors for Trigger " << m_id << ": File " << m_sffile
              << ", Histogram " << histoname;
    TFile file(m_sffile.c_str(), "READONLY");
    TH2F* sfhisto = (TH2F*)file.Get(histoname.c_str());
    
    // Get the pT and eta bin borders
    for (int ix = 0; ix <= sfhisto->GetNbinsX(); ++ix){
        m_xbins.emplace_back(2 * sfhisto->GetXaxis()->GetBinCenter(ix) -
                             sfhisto->GetXaxis()->GetBinLowEdge(ix));
    }
    for (int iy = 0; iy <= sfhisto->GetNbinsY(); ++iy){
        m_ybins.emplace_back(2 * sfhisto->GetYaxis()->GetBinCenter(iy) -
                             sfhisto->GetYaxis()->GetBinLowEdge(iy));
    }
    // Fill the m_sf array with the values from the root histo
    //for (int i = 0; i < runcount; i++){
    //LOG(INFO) << "x first border no." << 0 << ": "<< m_xbins[0];
    for (int ix = 1; ix <= sfhisto->GetNbinsX(); ix++) {
        for (int iy = 1; iy <= sfhisto->GetNbinsY(); iy++) {
            m_sf[0][ix - 1][iy - 1] = static_cast<float>(
            sfhisto->GetBinContent(ix, iy) - error_multiplier * sfhisto->GetBinError(ix, iy));
            //LOG(INFO) << "value: " << m_sf[0][ix-1][iy-1];
            //LOG(INFO) << "x upper border no."<< ix << ": "<< m_xbins[ix];
        }
    }    
    file.Close();
}

void LeptonTriggerSFProducer::Produce(ZJetEvent const& event,
                                 ZJetProduct& product,
                                 ZJetSettings const& settings) const
{
    if(product.m_zValid){
        product.m_weights["mu1TriggerSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.first);
        product.m_weights["mu2TriggerSFWeight"] = 1/GetScaleFactor(0, *product.m_zLeptons.second);
        product.m_weights["leptonTriggerSFWeight"] =
            1/(1-(1-GetScaleFactor(0, *product.m_zLeptons.first)) * (1-GetScaleFactor(0, *product.m_zLeptons.second)));
    }
    else
        product.m_weights["leptonTriggerSFWeight"] = 0;
}
