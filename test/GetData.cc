// -*- C++ -*-
//
// Package:    GetData
// Class:      GetData
// 
/**\class GetData GetData.cc RecoLocalCalo/GetData/src/GetData.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Emmanuelle PEREZ
//         Created:  Wed Apr  4 09:15:08 CEST 2007
// $Id$
//
//


// system include files
#include <memory>
#include <vector>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include <FWCore/Framework/interface/Handle.h>
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EBDataFrame.h"
#include "DataFormats/EcalDigi/interface/EEDataFrame.h"
#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
using namespace edm;
using namespace std;

//
// class decleration
//

class GetData : public edm::EDAnalyzer {
   public:
  explicit GetData(const edm::ParameterSet&);
  ~GetData();
  
   private:
  virtual void beginJob(const edm::EventSetup&) ;
  void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag EBdigiCollection_;
  edm::InputTag EEdigiCollection_;
  edm::InputTag ESdigiCollection_;      
  // ----------member data ---------------------------
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
GetData::GetData(const edm::ParameterSet& ps)
{

  EBdigiCollection_ = ps.getParameter<edm::InputTag>("EBdigiCollection");
  EEdigiCollection_ = ps.getParameter<edm::InputTag>("EEdigiCollection");
  ESdigiCollection_ = ps.getParameter<edm::InputTag>("ESdigiCollection");
}


GetData::~GetData()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
GetData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

    edm::Handle< DetSetVector<PixelDigi> >  input;
    iEvent.getByLabel("siPixelDigis", input);
     auto_ptr<DetSetVector<PixelDigi> > NewPixelDigi(new DetSetVector<PixelDigi> );
     DetSetVector<PixelDigi>* tt = NewPixelDigi.get();
     *tt = *input.product();

     edm::Handle< edm::DetSetVector<SiStripDigi> >  input2;
     iEvent.getByLabel("siStripDigis",input2);
     auto_ptr<DetSetVector<SiStripDigi> > NewSiDigi(new DetSetVector<SiStripDigi> );
     DetSetVector<SiStripDigi>* uu = NewSiDigi.get();
     *uu = *input2.product();

     
     Handle<EBDigiCollection> EcalDigiEB;
     Handle<EEDigiCollection> EcalDigiEE;
     Handle<ESDigiCollection> EcalDigiES;
     const EBDigiCollection* EBdigis =0;
     const EEDigiCollection* EEdigis =0;
     const ESDigiCollection* ESdigis =0; 
  
  
     iEvent.getByLabel( EBdigiCollection_, EcalDigiEB );
     EBdigis = EcalDigiEB.product();
     LogDebug("DigiInfo") << "total # EBdigis: " << EBdigis->size() ;
     
     iEvent.getByLabel( EEdigiCollection_, EcalDigiEE );
     EEdigis = EcalDigiEE.product();
     LogDebug("DigiInfo") << "total # EEdigis: " << EEdigis->size() ;
    
    iEvent.getByLabel( ESdigiCollection_, EcalDigiES );
    ESdigis = EcalDigiES.product();
    LogDebug("DigiInfo") << "total # ESdigis: " << ESdigis->size() ;
   



}

// ------------ method called once each job just before starting event loop  ------------
void 
GetData::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GetData::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(GetData);
