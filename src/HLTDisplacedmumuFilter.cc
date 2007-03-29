#include "HLTrigger/btau/interface/HLTDisplacedmumuFilter.h"

#include <iostream>

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


using namespace edm;
using namespace reco;
using namespace std; 

//
// constructors and destructor
//
HLTDisplacedmumuFilter::HLTDisplacedmumuFilter(const edm::ParameterSet& iConfig)
{
	//now do what ever initialization is needed
	minLxySignificance_ = iConfig.getParameter<double>("MinLxySignificance");       
	maxNormalisedChi2_ = iConfig.getParameter<double>("MaxNormalisedChi2");  
	minCosinePointingAngle_ = iConfig.getParameter<double>("MinCosinePointingAngle");
	src_ = iConfig.getParameter<edm::InputTag>("Src");
	produces<VertexCollection>();
}


HLTDisplacedmumuFilter::~HLTDisplacedmumuFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// ------------ method called once each job just before starting event loop  ------------
void HLTDisplacedmumuFilter::beginJob(const edm::EventSetup&)
{

}

// ------------ method called once each job just after ending the event loop  ------------
void HLTDisplacedmumuFilter::endJob() 
{
 	
}

// ------------ method called on each new Event  ------------
bool HLTDisplacedmumuFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

			
	//get the jpsi collection 
	
	Handle<CandidateCollection> candCollection;
	iEvent.getByLabel(src_,candCollection);
	
	//get the transient track builder:
	edm::ESHandle<TransientTrackBuilder> theB;
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
	std::auto_ptr<VertexCollection> vertexCollection(new VertexCollection());
	
	if(candCollection->size()>0) {
		CandidateCollection::const_iterator it=(*candCollection).begin();
		
		for (;it!=(*candCollection).end();it++) {
		
			if ((*it).numberOfDaughters()==2){
			
				vector<TransientTrack> t_tks;
				
				Candidate::const_iterator dit=(*it).begin();
				
				for (;dit!=(*it).end();dit++){
				
				const RecoCandidate * cand = dynamic_cast<const RecoCandidate *> (&(*dit));
				TrackRef tk = (cand)->track();
				
				TransientTrack ttkp   = (*theB).build(&tk);
				
				t_tks.push_back(ttkp);
				
				}
				
				
				// Call the KalmanVertexFitter if 2 tracks
				if (t_tks.size()==2){
				
					KalmanVertexFitter kvf;
					TransientVertex tv = kvf.vertex(t_tks);
					if (tv.isValid()) {
						Vertex vertex = tv;
					
						
						GlobalPoint v = tv.position();
						GlobalError err = tv.positionError();
						
						float lxy = v.perp();
						float lxyerr = err.rerr(v);
						
						float normChi2 = tv.normalisedChiSquared();
					
						
					//     float cosAlpha = v.Dot((*it).momentum())/(v.R()*(*it).momentum().R());
					
						Vertex::Point vperp(v.x(),v.y(),0.);
						math::XYZVector pperp((*it).momentum().x(),(*it).momentum().y(),0.);
					
						float cosAlpha = vperp.Dot(pperp)/(vperp.R()*pperp.R());
						if( (cosAlpha > minCosinePointingAngle_) && (normChi2 < maxNormalisedChi2_) && ( lxy/lxyerr > minLxySignificance_ ) ){
	// 						Vertex vertex = tv.vertex();
							vertexCollection->push_back(vertex);
							iEvent.put(vertexCollection);
							return true;
						}
					}
				}
			}	
		}	
	}
	return false;
}

