// -*- C++ -*-
//
// Package:    JetCrap/RecHitAnalyser
// Class:      RecHitAnalyser
//
/**\class RecHitAnalyser RecHitAnalyser.cc JetCrap/RecHitAnalyser/plugins/RecHitAnalyser.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Robin Cameron Aggleton
//         Created:  Mon, 01 Aug 2016 17:26:00 GMT
//
//


// system include files
#include <memory>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"

// EDM formats
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"

#include "DataFormats/Common/interface/SortedCollection.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class RecHitAnalyser : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit RecHitAnalyser(const edm::ParameterSet&);
      ~RecHitAnalyser();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      void storeHFRecHits(edm::Handle<HFRecHitCollection> recHits);
      void storeHBHERecHits(edm::Handle<HBHERecHitCollection> recHits);
      void storePFRecHits(edm::Handle<reco::PFRecHitCollection> recHits);

      // ----------member data ---------------------------

      edm::Service<TFileService> fs_;
      TH2F * recHitMap_;
      TH2F * pfRecHitMap_;

      // EDM input tags
      edm::EDGetTokenT<HFRecHitCollection> recHitTokenHF_;
      edm::EDGetTokenT<HBHERecHitCollection> recHitTokenHBHE_;
      edm::EDGetTokenT<reco::PFRecHitCollection> pfRecHitTokenHBHE_;
      edm::EDGetTokenT<reco::PFRecHitCollection> pfRecHitTokenHF_;
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
RecHitAnalyser::RecHitAnalyser(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");

  recHitTokenHBHE_ = consumes<HBHERecHitCollection>(iConfig.getUntrackedParameter("recHitTokenHBHE", edm::InputTag("")));
  recHitTokenHF_ = consumes<HFRecHitCollection>(iConfig.getUntrackedParameter("recHitTokenHF", edm::InputTag("")));

  pfRecHitTokenHBHE_ = consumes<reco::PFRecHitCollection>(iConfig.getUntrackedParameter("pfRecHitTokenHBHE", edm::InputTag("")));
  pfRecHitTokenHF_ = consumes<reco::PFRecHitCollection>(iConfig.getUntrackedParameter("pfRecHitTokenHF", edm::InputTag("")));

  recHitMap_ = fs_->make<TH2F>("recHitMap", "Energy in GeV;ieta;iphi", 85, -42, 43, 73, 0, 73);
  pfRecHitMap_ = fs_->make<TH2F>("pfRecHitMap", "Energy in GeV;ieta;iphi", 85, -42, 43, 73, 0, 73);

}


RecHitAnalyser::~RecHitAnalyser()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
RecHitAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using std::cout;
  using std::endl;

  Handle<HBHERecHitCollection> recHitsHBHE;
  iEvent.getByToken(recHitTokenHBHE_, recHitsHBHE);

  Handle<HFRecHitCollection> recHitsHF;
  iEvent.getByToken(recHitTokenHF_, recHitsHF);

  Handle<reco::PFRecHitCollection> pfRecHitsHBHE;
  iEvent.getByToken(pfRecHitTokenHBHE_, pfRecHitsHBHE);

  Handle<reco::PFRecHitCollection> pfRecHitsHF;
  iEvent.getByToken(pfRecHitTokenHF_, pfRecHitsHF);

  if (recHitsHBHE.isValid()) {
    storeHBHERecHits(recHitsHBHE);
  } else {
    cout << "Not analysing HBHE" << endl;
  }

  if (recHitsHF.isValid()) {
    cout << "HF rec hits ok" << endl;
    storeHFRecHits(recHitsHF);
  } else {
    cout << "Not analysing HF" << endl;
  }

  if (pfRecHitsHBHE.isValid()) {
    cout << "HBHE PF RecHits OK" << endl;
    storePFRecHits(pfRecHitsHBHE);
  } else {
    cout << "Not doing HBHE PF rechits" << endl;
  }

  if (pfRecHitsHF.isValid()) {
    cout << "HF PF RecHits OK" << endl;
    storePFRecHits(pfRecHitsHF);
  } else {
    cout << "Not doing HF PF rechits" << endl;
  }
}


void RecHitAnalyser::storeHBHERecHits(edm::Handle<HBHERecHitCollection> recHits) {
  using namespace std;
  for (auto& rh: *recHits) {
    cout << rh << endl;
    HcalDetId id = rh.id();
    recHitMap_->Fill(id.ieta(), id.iphi(), rh.energy());
  }
}

void RecHitAnalyser::storeHFRecHits(edm::Handle<HFRecHitCollection> recHits) {
  using namespace std;
  for (auto& rh: *recHits) {
    cout << rh << endl;
    HcalDetId id = rh.id();
    recHitMap_->Fill(id.ieta(), id.iphi(), rh.energy());
  }
}

void RecHitAnalyser::storePFRecHits(edm::Handle<reco::PFRecHitCollection> recHits) {
  using namespace std;
  for (auto& rh: *recHits) {
    cout << rh << endl;
  }
}


// ------------ method called once each job just before starting event loop  ------------
void
RecHitAnalyser::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
RecHitAnalyser::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RecHitAnalyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecHitAnalyser);
