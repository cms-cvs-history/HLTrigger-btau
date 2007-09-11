#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HLTrigger/btau/test/stubs/bLifetimeHLTanal.h"
//#include "HLTrigger/btau/test/stubs/TruthDrawer.h"

#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "PhysicsTools/Utilities/interface/EtComparator.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"

//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"


bLifetimeHLTanal::bLifetimeHLTanal(const edm::ParameterSet& iConfig)
{

  // Read configuration parameters.  

  string outName = iConfig.getUntrackedParameter<string>("outputHistos", string("bLifetimeHLTanal.root"));
  selectUDS_ = iConfig.getParameter<bool>("selectUDS");

  // Book histograms.
  outFile_ = new TFile(outName.c_str(), "recreate");
  outFile_->cd();

  // Events selected by MC truth filter.
  eventsHis_ = new TH1F("MCfilter","MC truth filter",2,-0.5,1.5);

  // Trigger decisions.
  l1His_  = new TH1F("L1", "L1 results", 10,-0.5,9.5);
  hltHis_ = new TH1F("HLT","HLT results",10,-0.5,9.5);

  // Jet Pt
  for (unsigned int i = 0; i < nJets_; i++) {
    ostringstream nameTmp, titleTmp;
    nameTmp << "Jet"<<i+1<<ends;
    titleTmp << "Jet "<<i+1<<" Pt"<<ends; 
    ptJetHis_[i] = new TH1F(nameTmp.str().c_str(), titleTmp.str().c_str(), 25, 0.0, 250.0);
  } 

  // Transverse energy
  etHis_ = new TH1F("et", "transverse energy", 20, 0.0, 800.0);

  // b tag
  for (unsigned int i = 0; i < nJets_; i++) {
    ostringstream nameTmp, titleTmp;
    nameTmp << "Jet"<<i+1<<"bL25"<<ends;
    titleTmp << "Jet "<<i+1<<" L2.5 b tag"<<ends; 
    bTagL25His_[i] = new TH1F(nameTmp.str().c_str(), titleTmp.str().c_str(), 20, -15.0, 25.0);
  }

  for (unsigned int i = 0; i < nJets_; i++) {
    ostringstream nameTmp, titleTmp;
    nameTmp << "Jet"<<i+1<<"bL3"<<ends;
    titleTmp << "Jet "<<i+1<<" L3 b tag"<<ends; 
    bTagL3His_[i] = new TH1F(nameTmp.str().c_str(), titleTmp.str().c_str(), 20, -15.0, 25.0);
  } 
}

bLifetimeHLTanal::~bLifetimeHLTanal() {

  outFile_->Write();

  outFile_->cd();
  outFile_->ls();
  delete etHis_;
  for (unsigned int i = 0; i < nJets_; i++) {
    delete ptJetHis_[i];
  }
  outFile_->Close();
  delete outFile_;
}

void bLifetimeHLTanal::endJob()
{
  // Print results.
  // This shows the number of times a given module was the last
  // run in a given trigger path. This equals the number of rejected
  // events for all but the final trigger level. For that, it is just
  // the number of times the final trigger module was run.
  map<string, map<unsigned int, unsigned int> >::const_iterator m;
  cout<<endl<<"MODULE RESULTS"<<endl;
  for (m = modules_.begin(); m != modules_.end(); m++) {
    cout<<m->first;
    map<unsigned int, unsigned int>::const_iterator n;
    for (n = m->second.begin(); n != m->second.end(); n++) {
      cout<<" "<<n->first<<" ("<<n->second<<")";
    }
    cout<<endl;
  }
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
bLifetimeHLTanal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //  using namespace reco;
  using namespace std;
 
  static unsigned int nEvent = 0;
  cout<<endl<<"Event "<<++nEvent<<endl;
  bool passL1  = this->analyzeL1(iEvent, iSetup);
  bool passHLT = this->analyzeHLT(iEvent, iSetup);
  cout<<" L1="<<passL1<<" HLT="<<passHLT<<endl;
  unsigned int flavour = this->analyzeTruth(iEvent, iSetup);
  bool select = selectUDS_ && flavour <=3 || ! selectUDS_;
  eventsHis_->Fill(0.);
  if (select) {
    eventsHis_->Fill(1.);
    if (passL1) {
      this->analyzeReco(iEvent, iSetup);
    }
  }
}

bool bLifetimeHLTanal::analyzeL1(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //  using namespace reco;
  using namespace std;

  bool passL1 = false;
  // Give each trigger a code number.
  // Names come from HLTrigger/btau/jetTag/lifetime_L1Seeds.cff
  map<string, float> l1Bits;
  l1Bits["all"]             = 0.;
  l1Bits["L1_SingleJet150"] = 1.;
  l1Bits["L1_DoubleJet100"] = 2.;
  l1Bits["L1_TripleJet50"]  = 3.;
  l1Bits["L1_QuadJet30"]    = 4.;
  l1Bits["L1_HTT300"]       = 5.;
  l1Bits["inputEvents"]     = 9.;

  // get hold of L1ParticleMapCollection
  Handle<l1extra::L1ParticleMapCollection> l1Handle;
  
  try {
    iEvent.getByLabel("l1extraParticleMap", l1Handle);

    const l1extra::L1ParticleMapCollection& l1 = *l1Handle;

    //    cout<<endl<<"L1 Decision"<<endl;

    for (unsigned int i = 0; i != l1.size(); i++) { 
      const string name = l1[i].triggerName();
      //      cout<<i<<" "<<name<<" "<<l1[i].triggerDecision()<<endl;
      map<string, float>::const_iterator l1BitCode; 
      l1BitCode = l1Bits.find(name);
      if (l1BitCode != l1Bits.end()) { 
        if (l1[i].triggerDecision()) {
          passL1 = true;
          l1His_->Fill(l1BitCode->second);
        }
      }
    }
  }

  catch (edm::Exception) {cout<<"No L1 found"<<endl;}

  // Total L1 pass rate.
  if (passL1) l1His_->Fill(l1Bits["all"]);
  // Total input event rate to L1.
  l1His_->Fill(l1Bits["inputEvents"]);

  return passL1;
}

bool bLifetimeHLTanal::analyzeHLT(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //  using namespace reco;
  using namespace std;

  bool passHLT = false;
  // Give each trigger a code number.
  // Names come from HLTrigger/btau/jetTag/data/lifetimeTriggerSequences.cff
  map<string, float> hltBits;
  hltBits["all"]         = 0.;
  hltBits["HLTB1Jet"]    = 1.;
  hltBits["HLTB2Jet"]    = 2.;
  hltBits["HLTB3Jet"]    = 3.;
  hltBits["HLTB4Jet"]    = 4.;
  hltBits["HLTBHT"]      = 5.;
  hltBits["inputEvents"] = 9.;

  edm::Handle<edm::TriggerResults> hltHandle;
  try {
    iEvent.getByLabel("TriggerResults", hltHandle);

    const edm::TriggerResults& hlt = *hltHandle;
    TriggerNames hltNames(hlt);

    const Provenance& prov = iEvent.getProvenance(hltHandle.id());
    const string& procName = prov.processName();

    //    cout<<endl<<"HLT Decision"<<endl;

    for (unsigned int j = 0; j != hlt.size(); j++) { 
      //      string name = hlt.name(j);
      string name = hltNames.triggerName(j);
      //      cout<<j<<" "<<tns_.getTrigPath[j]<<" "<<tns_.getTrigPathModule[j, hlt[j].index [j]]<<" "<<hlt[j].state()<<endl;
      if (hlt[j].accept()) cout<<j<<" "<<name<<" passed"<<endl;
      map<string, float>::const_iterator hltBitCode; 
      hltBitCode = hltBits.find(name);
      if (hltBitCode != hltBits.end()) { 
        if (hlt[j].accept()) {
          passHLT = true;
          hltHis_->Fill(hltBitCode->second);
        }
      }
    }
  }
  catch (edm::Exception) {cout<<"No HLT found"<<endl;}  

  // Total HLT pass rate.
  if (passHLT) hltHis_->Fill(hltBits["all"]);
  // Total input event rate to HLT.
  hltHis_->Fill(hltBits["inputEvents"]);

  return passHLT;
}


unsigned int
bLifetimeHLTanal::analyzeTruth(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //  using namespace reco;
  using namespace std;

  edm::Handle<reco::CandidateCollection> MCparticles;
  iEvent.getByLabel("genParticleCandidates", MCparticles);

  bool found = false;
  unsigned int heaviestFlavour = 999;
  reco::CandidateCollection::const_iterator p;
  for (p = MCparticles->begin(); p != MCparticles->end(); p++) {
    unsigned int particleType = abs(p->pdgId());
    // Check if gluon
    if (particleType == 21) particleType = 0;
    //    int status = p->status(); // In CMSSW 1.4 onwards.
    //    int status = reco::status(*p); // In CMSSW 1.3.
    // Check if this is a gluon or quark.
    if (particleType >= 0 && particleType <= 6) {
      //      cout<<"debug "<<nEvent<<" "<<p->pdgId()<<" "<<p->et()<<" "<<status;
      if (p->et() > 10. ) {
        if (!found || heaviestFlavour < particleType ) {
          found = true;
          heaviestFlavour = particleType;
        }
      }
    }
  }

  unsigned int foundFlavour;
  if (found) {
    foundFlavour = heaviestFlavour;
  }  else {
    foundFlavour = 7;
  }

  /*
  if (found) cout<<"Flavour "<<heaviestFlavour<<endl;
  if (found && heaviestFlavour <= 3) {
    TruthDrawer truth;
    truth.analyze(iEvent, iSetup);
  }
  */

  return foundFlavour;
}

void 
bLifetimeHLTanal::analyzeReco(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  using namespace std;

  // Jet Et
  try {
    Handle<CaloJetCollection> jetsHandle;
    iEvent.getByLabel("MCJetCorJetIcone5", jetsHandle);
    CaloJetCollection jets = *jetsHandle;
    // Sort jets in decreasing Et.
    GreaterByEt<CaloJet> etSort;
    sort(jets.begin(), jets.end(), etSort);
       
    for (unsigned int j = 0; j < jets.size(); j++) {
      if (j < nJets_) {
        ptJetHis_[j]->Fill(jets[j].pt());
	//	cout<<"jet "<<j<<" = "<<jets[j].pt()<<endl;
      }
    }
  }
  catch (edm::Exception) {
    cout<<"No Good Jet Et found"<<endl;
  try {
    Handle<CaloJetCollection> jetsHandle;
        iEvent.getByLabel("iterativeCone5CaloJets", jetsHandle);
    CaloJetCollection jets = *jetsHandle;
    // Sort jets in decreasing Et.
    GreaterByEt<CaloJet> etSort;
    sort(jets.begin(), jets.end(), etSort);
       
    for (unsigned int j = 0; j < jets.size(); j++) {
      if (j < nJets_) {
        ptJetHis_[j]->Fill(jets[j].pt());
	//	cout<<"jet "<<j<<" = "<<jets[j].pt()<<endl;
      }
    }
  }
  catch (edm::Exception) {
    cout<<"No Good or Bad Jet Et found"<<endl;}
  }
  /*
  // Check consistent with jets stored in trigger objects.
  try {
    Handle<HLTFilterObjectWithRefs> hltL2b4jetHandle;
    iEvent.getByLabel("hltL2b4jet", hltL2b4jetHandle);
    const HLTFilterObjectWithRefs& hltL2b4jet = *hltL2b4jetHandle;
    cout<<"nhltjets "<<hltL2b4jet.size()<<endl;
    for (unsigned int j = 0; j < hltL2b4jet.size(); j++) {
        cout<<"hltj "<<j<<" = "<<hltL2b4jet[j].pt()<<endl;
    }
  }
  catch (edm::Exception) {cout<<"No HLT Jet found"<<endl;}
  */

  // Event transverse energy
  try {
    Handle<METCollection> etHandle;
    iEvent.getByLabel("htMet", etHandle);
    if (etHandle->size() != 1) cout<<"ERROR: Et size not 1 !"<<endl;
    etHis_->Fill(etHandle->begin()->sumEt());
    //    cout<<"HTT = " << etHandle->begin()->sumEt()<<endl;
  }
  catch (edm::Exception) {cout<<"No transverse energy found"<<endl;}

  /*
  // Check consistent with transverse energy stored in trigger objects.
  try {
    Handle<HLTFilterObjectWithRefs> hltL2bHTHandle;
    iEvent.getByLabel("hltL2bHT", hltL2bHTHandle);
    cout<<"nhltHT "<<hltL2bHTHandle->size()<<endl;
    if (hltL2bHTHandle->size() > 0) {
      const RefToBase<Candidate>& candHT = hltL2bHTHandle->getParticleRef(0);
      METRef htref = candHT.castTo<METRef>();
      //      METRef htref= candHT.get<METRef>();

      cout<<"hltHTT = " << htref->sumEt() << endl;
    }
  }
  catch (edm::Exception) {cout<<"No HLT HT found"<<endl;}
  */

  // L2.5 b tag
  try {
    Handle<JetTagCollection> jetsHandle25;
    iEvent.getByLabel("hltLifetimeJetTagsL25", jetsHandle25);
    const JetTagCollection& jets25 = *jetsHandle25;
  
    for (unsigned int j = 0; j < nJets_; j++) {
      // Find best b tag using first j jets.
      double bestTag = -99.;    
      for (unsigned int k = 0; k < jets25.size(); k++) {
        if (k <= j) {
          double bTag = jets25[k].discriminator();
          if (bestTag < bTag) bestTag = bTag;
        }
      }
      bTagL25His_[j]->Fill(bestTag);
    }
  }
  catch (edm::Exception) {cout<<"No L2.5 b tag found"<<endl;}

  // L3 b tag
  try {
    Handle<JetTagCollection> jetsHandle3;
    iEvent.getByLabel("hltLifetimeJetTagsL3", jetsHandle3);
    const JetTagCollection& jets3 = *jetsHandle3;
   
    for (unsigned int j = 0; j < nJets_; j++) {
      // Find best b tag using first j jets.
      double bestTag = -99.;    
      for (unsigned int k = 0; k < jets3.size(); k++) {
        if (k <= j) {
          double bTag = jets3[k].discriminator();
          if (bestTag < bTag) bestTag = bTag;
        }
      }
      bTagL3His_[j]->Fill(bestTag);
    }
  }
  catch (edm::Exception) {cout<<"No L3 b tag found"<<endl;}
}
