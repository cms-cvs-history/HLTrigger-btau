#ifndef bLifetimeHLTanal_h
#define bLifetimeHLTanal_h

/** \class bLifetimeHLTanal 
 *
 * This class analyses the results of the b-lifetime HLT path.
 *
 * Author: Ian Tomalin
 * Date:   5th June 2007
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "TFile.h"
#include "TH1.h"

//#include "FWCore/Framework/interface/TriggerNamesService.h"

#include <string>
#include <map>

using namespace std;
using namespace edm;

class bLifetimeHLTanal : public edm::EDAnalyzer {

   public:
      explicit bLifetimeHLTanal(const edm::ParameterSet&);
      ~bLifetimeHLTanal();

   private:
      virtual void beginJob(const edm::EventSetup&) {}
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual bool analyzeL1(const edm::Event&, const edm::EventSetup&);
      virtual bool analyzeHLT(const edm::Event&, const edm::EventSetup&);
      virtual unsigned int analyzeTruth(const edm::Event&, const edm::EventSetup&);
      virtual void analyzeReco(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();

   private:
  //      edm::service::TriggerNamesService tns_;
  // Number of times each module was last called on trigger path.
      map<string, map<unsigned int, unsigned int> > modules_;

  bool selectUDS_;

  // Histograms
  TFile* outFile_; 
  enum bLifetimeHLTanal_const {nJets_ = 4};
  TH1F* eventsHis_;
  TH1F* l1His_;
  TH1F* hltHis_;
  TH1F* ptJetHis_[nJets_]; 
  TH1F* etHis_; 
  TH1F* bTagL25His_[nJets_]; 
  TH1F* bTagL3His_[nJets_]; 
};

#endif // bLifetimeHLTanal_h
