// -*- C++ -*-
//
// Package:    HLTAn
// Class:      HLTAn
// 
/**\class HLTAn HLTAn.cc Analysis/HLTAn/src/HLTAn.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Aram Avetisyan
//         Created:  Thu May 17 12:24:24 CDT 2007
// $Id$
//
//

// system include files
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "L1Trigger/GlobalMuonTrigger/test/L1MuGMTTree.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTConfig.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/MuonReco/interface/Muon.h"

//Need this to get at the HLTFilter objects
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
//Tracking object
#include "DataFormats/TrackReco/interface/Track.h"
//Trigger Results
#include "DataFormats/Common/interface/TriggerResults.h"
//Jets
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <algorithm>
#include <vector>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <cmath>
#include <TH1F.h>
#include <TH2F.h>
#include <TAxis.h>
#include <TMath.h>

using namespace edm;
using namespace std;

const int NJETS    = 4;
const int SENTINEL = -1;
const int NPATHS   = 10;
const int NMUPATHS = 5;

string PathNames[NPATHS] = {"HLTB1Jet","HLTB2Jet","HLTB3Jet","HLTB4Jet","HLTBHT",
			    "HLTB1JetMu","HLTB2JetMu","HLTB3JetMu","HLTB4JetMu","HLTBHTMu"};

//Minumum pt for the histograms is (L2 ptcut - 5) GeV except for HT paths
int HistMinPt[NPATHS] = {175, 115, 65, 35, 200, 45, 115, 65, 35, 200};

int i, j, k;

//const unsigned int  nL1(l1extra::L1ParticleMap::kNumOfL1TriggerTypes);  
int nlevel1;
//
// class decleration
//

class HLTAn : public EDAnalyzer {
   public:
      explicit HLTAn(const ParameterSet&);
      ~HLTAn();


   private:
      virtual void beginJob(const EventSetup&) ;
      virtual void analyze(const Event&, const EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  
  //Muons and related jets
  InputTag  L2Muons_, L3Muons_, hltbmuL1Jets_;
  //Reco Jets
  InputTag  recoJets5_;
  //HLT and L1 results
  InputTag  TrigRes_; 
  InputTag  l1ParticleMapTag_;   // Only because decisionWord of L1GTRR is not yet filled!
  InputTag  l1GTReadoutRecTag_;  // L1GlobalTriggerReadoutRecord
  //Jets
  vector<InputTag> hltBjets_;

  string OutputFile_;
  vector<unsigned int> l1Accepts_; // number of events accepted by each L1 algorithm
  vector<string> l1Names_;    // name of each L1 algorithm
  bool init_;                           // vectors initialised or not
  
  string l1Name, strin1, strin2;

  int npath1;
  double xtrig, xtot;
  double ptrel;
  int ntot;
  int l2mu; 
  int l3mu; 
  int nl1_mu7j15;
  int nSJ150level1, nDJ100level1, nTJ50level1, nQJ30level1, nHT300level1;
  int nPassPath[NPATHS];
  int ntrigs, itrig;
  int nL1;
  bool accept, passany, passb;
  double HT;
  double ptjet[NPATHS][NJETS];
  stringstream tempstr1, tempstr2;
  
  int errmu2, errmu3, errjet[NPATHS], errjet3dR, errbmu;
  int errjetl1, errSLL3, errRjets;

  TFile *outFile;
  
  TH1F *hTotalEv;
  TH1F *hL1JetPt;
  
  //Path 1
  
  //Muons for bmu paths
  TH1F *hPnL2MuPt[NMUPATHS];
  TH1F *hPnL3MuPt[NMUPATHS];
  
  //Jets and HT
  TH1F *hPnJetPt[NPATHS];
  
  //L1 Paths
  TH1F *hP1L1trig;
  TH1F *hHTL1trig;
  TH1F *hSJ150L1trig;
  TH1F *hDJ100L1trig;
  TH1F *hTJ50L1trig;
  TH1F *hQJ30L1trig;
  TH1F *hHT300L1trig;

  //HT Path
  TH1F *hHTJetPtrel;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HLTAn::HLTAn(const ParameterSet& iConfig)
{
  //now do what ever initialization is needed
//   cout<<"Begin initialization"<<endl;
  
  //Muons and related jets
  L2Muons_           = iConfig.getParameter<InputTag >("L2Muons");
  L3Muons_           = iConfig.getParameter<InputTag >("L3Muons");
  hltbmuL1Jets_      = iConfig.getParameter<InputTag >("hltbmuL1Jets");
  //Reco Jets
  recoJets5_         = iConfig.getParameter<InputTag >("recoJets5");
  //HLT and L1 results
  TrigRes_           = iConfig.getParameter<InputTag >("TrigRes");
  l1ParticleMapTag_  = iConfig.getParameter<InputTag >("L1ExtraParticleMap");
  l1GTReadoutRecTag_ = iConfig.getParameter<InputTag >("L1GTReadoutRecord");

  //Jets
  hltBjets_          = iConfig.getParameter<vector<InputTag> >("hltBjets");
  
  npath1             = iConfig.getParameter<int >("npath1");
  OutputFile_        = iConfig.getParameter<string >("OutputFile");

  //Again, file before histograms
  outFile = new TFile(OutputFile_.c_str(), "RECREATE");
  outFile->cd();

  hL1JetPt  = new TH1F("hL1JetPt", "Jet Pt", 20, 0, 100);

  //L1 Paths
  hP1L1trig  = new TH1F("hP1L1trig", "Path 1 L1", 10, 0, 10);
  hHTL1trig  = new TH1F("hHTL1trig", "Path HT L1", 10, 0, 10);
  hSJ150L1trig  = new TH1F("hSJ150L1trig", "Path SJ L1", 10, 0, 10);
  hDJ100L1trig  = new TH1F("hDJ100L1trig", "Path DJ L1", 10, 0, 10);
  hTJ50L1trig  = new TH1F("hTJ50L1trig", "Path TJ L1", 10, 0, 10);
  hQJ30L1trig  = new TH1F("hQJ30L1trig", "Path QJ L1", 10, 0, 10);
  hHT300L1trig  = new TH1F("hHT300L1trig", "Path HT300 L1", 10, 0, 10);
  
  for (i = 0; i < NPATHS; i++){
    j = i % 5;
    
    if (i == 4 || i == 9){
      tempstr1<<"h"<<PathNames[i]<<"HT";
      tempstr2<<"HT for path "<<PathNames[i]<<" at L2";
      strin1 = tempstr1.str(); strin2 = tempstr2.str();
      hPnJetPt[i]  = new TH1F(strin1.c_str(), strin2.c_str(), 40, HistMinPt[i], HistMinPt[i] + 800);
    }
    else{
      tempstr1<<"h"<<PathNames[i]<<"JetPt";
      tempstr2<<"Jet Pt for path "<<PathNames[i]<<" at L2";
      strin1 = tempstr1.str(); strin2 = tempstr2.str();
      hPnJetPt[i]  = new TH1F(strin1.c_str(), strin2.c_str(), 40, HistMinPt[i], HistMinPt[i] + 200);
    }

    tempstr1.str("");        tempstr2.str("");

    if (i > 4){
      tempstr1<<"h"<<PathNames[i]<<"L2MuPt";
      tempstr2<<"L2 Mu Pt for path "<<PathNames[i];
      strin1 = tempstr1.str(); strin2 = tempstr2.str();
      tempstr1.str("");        tempstr2.str("");

      hPnL2MuPt[j] = new TH1F(strin1.c_str(), strin2.c_str(), 20, 0, 60); 
      
      tempstr1<<"h"<<PathNames[i]<<"L3MuPt";
      tempstr2<<"L3 Mu Pt for path "<<PathNames[i];
      strin1 = tempstr1.str(); strin2 = tempstr2.str();
      tempstr1.str("");        tempstr2.str("");
      
      hPnL3MuPt[j] = new TH1F(strin1.c_str(), strin2.c_str(), 20, 0, 60);
    }
      nPassPath[i] = 0;
  }
  
  //HT Path
  hHTJetPtrel = new TH1F("hHTJetPtrel"," HT L3 ptrel ", 50,0.,5.);

  hTotalEv = new TH1F("hTotalEv", "Events ", 10, 0., 10.); 

  ntot = 0;
  
  l2mu=0;   l3mu=0;;
  init_ = false;
  nlevel1=0;
  nl1_mu7j15=0;
  xtrig=0;
  xtot=0;
  nL1=0;
  nSJ150level1 = 0; nDJ100level1 = 0; nTJ50level1 = 0; nQJ30level1 = 0; nHT300level1 = 0;

//   cout<<"End initialization"<<endl;

}


HLTAn::~HLTAn()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HLTAn::analyze(const Event& iEvent, const EventSetup& iSetup)
{
   using namespace reco;

//    cout<<"Begin analysis"<<endl;
   
   errmu2 = 0; errmu3 = 0; errjet3dR = 0; 
   errbmu = 0; errSLL3=0; errjetl1=0; errRjets = 0;

   for (i = 0; i < NPATHS; i++){
     for (j = 0; j < NJETS; j++){
       ptjet[i][j] = 0;
     }
   }
   HT = 0;
   
//    cout<<"Analyzing Jets"<<endl;
   
   for (i = 0; i < NPATHS; i++){
     errjet[i] = 0;
     
     Handle<HLTFilterObjectWithRefs> hltJetHandle;
     try { iEvent.getByLabel(hltBjets_[i], hltJetHandle); } catch (...) {errjet[i] = SENTINEL; continue;}
     
//      cout<<"Got Handle"<<endl;
     
     if (i != 4 && i != 9){
       for (HLTFilterObjectWithRefs::const_iterator myiter = hltJetHandle->begin(); myiter != hltJetHandle->end(); ++myiter) {
	 for (j = 0; j < NJETS; j++){
	   if (myiter->pt() > ptjet[i][j]){
	     for (k = NJETS - 1; k > j; k--){
	       ptjet[i][k] = ptjet[i][k - 1];
	     }
	     ptjet[i][j] = myiter->pt();
	     break;
	   }
	 }
       }
     }
   }
   
   Handle<CaloJetCollection> recoJets5;
   try { iEvent.getByLabel(recoJets5_, recoJets5); } catch (...) {errRjets = SENTINEL;}
   
   if (errRjets != SENTINEL){
     for (CaloJetCollection::const_iterator myiter = recoJets5->begin(); myiter != recoJets5->end(); ++myiter) {
       HT += myiter->pt();
     }
   }
//    cout<<"Analyzing Muons"<<endl;

   Handle<TrackCollection> L2Muons;
   try { iEvent.getByLabel(L2Muons_, L2Muons); } catch (...) {errmu2 = SENTINEL;}
   
   Handle<TrackCollection> L3Muons;
   try { iEvent.getByLabel(L3Muons_, L3Muons); } catch (...) {errmu3 = SENTINEL;}

   Handle<TriggerResults> hltresults;
   try { iEvent.getByLabel(TrigRes_, hltresults); } catch (...) {cerr<<"No trigger results?!"<<endl; return;}

   Handle<HLTFilterObjectWithRefs> hltbmuL1Jets;
   try { iEvent.getByLabel(hltbmuL1Jets_, hltbmuL1Jets); } catch (...) {errjetl1 = SENTINEL;}

//=================================
//========================= L1 
//=================================

//    cout<<"Analyzing L1"<<endl;

// get hold of L1ParticleMapCollection
   Handle<l1extra::L1ParticleMapCollection> L1PMC;
   try {iEvent.getByLabel(l1ParticleMapTag_,L1PMC);} catch (...) {;}
   if (L1PMC.isValid()) {
     //cout  << "L1ParticleMapCollection contains " << L1PMC->size() << " maps." << endl;
   } else {
     cout << "L1ParticleMapCollection with label ["+l1ParticleMapTag_.encode()+"] not found!";
     return;
   }

   nL1 = L1PMC->size();

   if (!init_) {
    init_ = true;
    l1Names_.resize(nL1);
    l1Accepts_.resize(nL1);
    for (i = 0; i < nL1; i++) {
      l1Accepts_[i]=0;
      if (i < l1extra::L1ParticleMap::kNumOfL1TriggerTypes) {
        l1extra::L1ParticleMap::L1TriggerType type(static_cast<l1extra::L1ParticleMap::L1TriggerType>(i));
        l1Names_[i] = l1extra::L1ParticleMap::triggerName(type);
      } 
      else l1Names_[i]="@@NameNotFound??";
    }
   }
   
   xtrig = 0.;
   // decision for each L1 algorithm
   for (i = 0; i < nL1; i++){
     if ((*L1PMC)[i].triggerDecision()) l1Accepts_[i]++;
     if (i < l1extra::L1ParticleMap::kNumOfL1TriggerTypes){
       l1extra::L1ParticleMap::L1TriggerType type(static_cast<l1extra::L1ParticleMap::L1TriggerType>(i));
       l1Name=l1extra::L1ParticleMap::triggerName(type);
     }
     if (l1Name == "L1_Mu5_Jet15"){
       if ((*L1PMC)[i].triggerDecision()){
	 nl1_mu7j15++;
	 xtrig=1.0;
	 hP1L1trig->Fill(xtrig);
       }
     }
     if (l1Name == "L1_HTT250") {
       if ((*L1PMC)[i].triggerDecision()) {
	 xtrig=1.0;
	 hHTL1trig->Fill(xtrig);
	 nlevel1++;
       }
     }
     if (l1Name == "L1_SingleJet150") {
       if ((*L1PMC)[i].triggerDecision()) {
	 xtrig=1.0;
	 hSJ150L1trig->Fill(xtrig);
	 nSJ150level1++;
       }
     }
     if (l1Name == "L1_DoubleJet100") {
       if ((*L1PMC)[i].triggerDecision()) {
	 xtrig=1.0;
	 hDJ100L1trig->Fill(xtrig);
	 nDJ100level1++;
       }
     }
     if (l1Name == "L1_TripleJet50") {
       if ((*L1PMC)[i].triggerDecision()) {
	 xtrig=1.0;
	 hTJ50L1trig->Fill(xtrig);
	 nTJ50level1++;
       }
     }
     if (l1Name == "L1_QuadJet30") {
       if ((*L1PMC)[i].triggerDecision()) {
	 xtrig=1.0;
	 hQJ30L1trig->Fill(xtrig);
	 nQJ30level1++;
       }
     }
     if (l1Name == "L1_HTT300") {
       if ((*L1PMC)[i].triggerDecision()) {
	 xtrig=1.0;
	 hHT300L1trig->Fill(xtrig);
	 nHT300level1++;
       }
     }
   }
   
   if (errjetl1 != SENTINEL){
     for (HLTFilterObjectWithRefs::const_iterator myiter = hltbmuL1Jets->begin(); myiter != hltbmuL1Jets->end(); ++myiter) {
       hL1JetPt->Fill(myiter->pt());
     }
   }

//=================================
//========================= L1 
//=================================

//    cout<<"Analyzing HLT"<<endl;
   
   for (itrig = npath1; itrig < npath1 + NPATHS; itrig++){
     accept = hltresults->accept(itrig);
     if (accept){
       i = itrig - npath1; 
       nPassPath[i]++;
       
       if (errjet[i] != SENTINEL){
	 if      (i < 4)            hPnJetPt[i]->Fill(ptjet[i][i]);
	 else if (i == 4 || i == 9){
	   hPnJetPt[i]->Fill(HT);
// 	   if (HT < 200) cout<<ntot<<'\t'<<i<<'\t'<<HT<<endl;
	 }
	 else if (i > 4 && i < 9)   hPnJetPt[i]->Fill(ptjet[i][i - 5]);
       }
       
       if (i > 4){
	 if (errmu2 != SENTINEL){
	   TrackCollection::const_iterator myiter = L2Muons->begin();
	   hPnL2MuPt[i % 5]->Fill(myiter->pt());
	 }
	 //else cout<<"Bad errmu2"<<endl;
	 if (errmu3 != SENTINEL){
	   TrackCollection::const_iterator myiter = L3Muons->begin();
	   hPnL3MuPt[i % 5]->Fill(myiter->pt());
	 }
	 else cout<<"Bad errmu3"<<endl;
       }
     }
   }
   
   ntot++;
   
   xtot=1.0;
   hTotalEv->Fill(xtot);
   
//    cout<<"End analysis"<<endl;
}


// ------------ method called once each job just before starting event loop  ------------
void 
HLTAn::beginJob(const EventSetup&)
{

}

// ------------ method called once each job just after ending the event loop  ------------
void 
HLTAn::endJob() {
  
  using namespace std;
  
  for (i = 0; i < NPATHS; i++){
    j = npath1 + i;
    cout<<"HLT Bit: "<<j<<'\t'<<PathNames[i];
    if (i == 4 || i == 9) cout<<"   ";
    cout<<'\t'<<"Ntot: "<<ntot<<'\t'<<"Npassing: "<<nPassPath[i]<<'\t'<<"Efficiency: "
	<<(nPassPath[i] * 1.0)/ntot<<endl;
  }
  
  if (init_) {
    for (i = 0; i < nL1; i++) {
      cout << "L1T-Report "
           << right << setw(10) << i << " "
           << right << setw(10) << l1Accepts_[i] << " "
           << l1Names_[i] << "\n";
    }
  } 
  else  cout << "L1T-Report - No L1 GTRRs found!" << endl;
  
  if (outFile!=0) { // if there was a histogram file...
    outFile->Write(); // write out the histrograms
    delete outFile; // close and delete the file
    outFile=0; // set to zero to clean up
  }
}




//define this as a plug-in
DEFINE_FWK_MODULE(HLTAn);
