//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: LegendEventAction.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical/Legend/src/LegendEventAction.cc
/// \brief Implementation of the LegendEventAction class
//
//
#include "LegendAnalysis.hh"
#include "LegendEventAction.hh"
#include "LegendScintHit.hh"
#include "LegendPMTHit.hh"
#include "LegendUserEventInformation.hh"
#include "LegendTrajectory.hh"
#include "LegendRecorderBase.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendEventAction::LegendEventAction(LegendRecorderBase* r)
  : fRecorder(r),fSaveThreshold(0),fScintCollID(-1),fPMTCollID(-1),fVerbose(1),
   fPMTThreshold(0),fForcedrawphotons(false),fForcenophotons(false)
{
  fEventMessenger = new LegendEventMessenger(this);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendEventAction::~LegendEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendEventAction::BeginOfEventAction(const G4Event* anEvent){
 
  //New event, add the user information object
  G4EventManager::
    GetEventManager()->SetUserInformation(new LegendUserEventInformation);
  //SD is sensitive detector
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fScintCollID<0){
    fScintCollID=SDman->GetCollectionID("scintCollection");
  }
  if(fPMTCollID<0)
    fPMTCollID=SDman->GetCollectionID("pmtHitCollection");

  if(fRecorder)fRecorder->RecordBeginOfEvent(anEvent);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendEventAction::EndOfEventAction(const G4Event* anEvent){

  LegendUserEventInformation* eventInformation = (LegendUserEventInformation*)anEvent->GetUserInformation();
  TTree *tree = LegendAnalysis::Instance()->getTree();
  LTEvent* lEvent = (LTEvent*) LegendAnalysis::Instance()->getTree()->GetBranch("event");
 
  G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();
 
  G4int n_trajectories = 0;
  if(trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  // extract the trajectories and draw them
  if(G4VVisManager::GetConcreteInstance())
  {
    for (G4int i=0; i<n_trajectories; i++)
    {
      LegendTrajectory* trj = (LegendTrajectory*)((*(anEvent->GetTrajectoryContainer()))[i]);
      if(trj->GetParticleName()=="opticalphoton")
      {
        trj->SetForceDrawTrajectory(fForcedrawphotons);
        trj->SetForceNoDrawTrajectory(fForcenophotons);
      }
      trj->DrawTrajectory();
    }
  }

  LegendScintHitsCollection* scintHC;// = 0;
  LegendPMTHitsCollection* pmtHC = 0;
 // HC is hit count
  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
  //Get the hit collections
  if(hitsCE){
    if(fScintCollID>=0)scintHC = (LegendScintHitsCollection*)(hitsCE->GetHC(fScintCollID));
    if(fPMTCollID>=0) pmtHC = (LegendPMTHitsCollection*)(hitsCE->GetHC(fPMTCollID));
  }

  //Hits in scintillator
  if(scintHC)
  {
    int n_hit = scintHC->entries();
    G4cout<<n_hit<<" = Hits in Scintillator"<<G4endl;
    G4ThreeVector  eWeightPos(0.);
    G4double edep;
    G4double edepMax=0;

    for(int i=0;i<n_hit;i++)
    { //gather info on hits in scintillator
      edep=(*scintHC)[i]->GetEdep();
      eventInformation->IncEDep(edep); //sum up the edep
      eWeightPos += (*scintHC)[i]->GetPos()*edep;//calculate energy weighted pos
      if(edep>edepMax)
      {
        edepMax=edep;//store max energy deposit
        G4ThreeVector posMax=(*scintHC)[i]->GetPos();
        eventInformation->SetPosMax(posMax,edep);
      }
    }
    
    lEvent->TotE=eventInformation->GetEDep();

    G4cout<<" ********** TotE = "<< lEvent->TotE <<G4endl;
    if(eventInformation->GetEDep()==0.)
    {
      if(fVerbose>0)G4cout<<"\tNo hits in the scintillator this event."<<G4endl;
    }
    else
    {
      //Finish calculation of energy weighted position
      eWeightPos/=eventInformation->GetEDep();
      eventInformation->SetEWeightPos(eWeightPos);
      if(fVerbose>0)
      {
        G4cout << "\tEnergy weighted position of hits in Legend : "
               << eWeightPos/mm << G4endl;
      }
    }
    if(fVerbose>0)
    {
    G4cout << "\tTotal energy deposition in scintillator : "
           << eventInformation->GetEDep() / keV << " (keV)" << G4endl;
    }
  }
 
  if(pmtHC)
  {
    G4ThreeVector reconPos(0.,0.,0.);
    G4int pmts=pmtHC->entries();
    G4cout<<"pmt hit count = "<<pmts<<G4endl;
    //Gather info from all PMTs
    for(G4int i=0;i<pmts;i++)
    {
      eventInformation->IncHitCount((*pmtHC)[i]->GetPhotonCount());
      reconPos+=(*pmtHC)[i]->GetPMTPos()*(*pmtHC)[i]->GetPhotonCount();
      if((*pmtHC)[i]->GetPhotonCount()>=fPMTThreshold)
      {
        eventInformation->IncPMTSAboveThreshold();
      }
      else//wasnt above the threshold, turn it back off
      {
        (*pmtHC)[i]->SetDrawit(false);
      }
    }
 
    if(eventInformation->GetHitCount()>0)//dont bother unless there were hits
    {
      reconPos/=eventInformation->GetHitCount();
      if(fVerbose>0){
        G4cout << "\tReconstructed position of hits in Legend : "
               << reconPos/mm << G4endl;
      }
      eventInformation->SetReconPos(reconPos);
    }
    pmtHC->DrawAllHits();
  }

  if(fVerbose>0)
  {
    //End of event output. later to be controlled by a verbose level
    G4cout << "\tNumber of photons that hit PMTs in this event : "
           << eventInformation->GetHitCount() << G4endl;
    G4cout << "\tNumber of PMTs above threshold("<<fPMTThreshold<<") : "
           << eventInformation->GetPMTSAboveThreshold() << G4endl;
    G4cout << "\tNumber of photons produced by scintillation in this event : "
           << eventInformation->GetPhotonCount_Scint() << G4endl;
    G4cout << "\tNumber of photons produced by cerenkov in this event : "
           << eventInformation->GetPhotonCount_Ceren() << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
           << eventInformation->GetAbsorptionCount() << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
           << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
           << G4endl;
    G4cout << "Unacounted for photons in this event : "
           << (eventInformation->GetPhotonCount_Scint() +
               eventInformation->GetPhotonCount_Ceren() -
               eventInformation->GetAbsorptionCount() -
               eventInformation->GetHitCount() -
               eventInformation->GetBoundaryAbsorptionCount())
           << G4endl;
  }
  //If we have set the flag to save 'special' events, save here
  if(fSaveThreshold&&eventInformation->GetPhotonCount() <= fSaveThreshold)
    G4RunManager::GetRunManager()->rndmSaveThisEvent();

  if(fRecorder)fRecorder->RecordEndOfEvent(anEvent);
  tree->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendEventAction::SetSaveThreshold(G4int save){
/*Sets the save threshold for the random number seed. If the number of photons
generated in an event is lower than this, then save the seed for this event
in a file called run###evt###.rndm
*/
  fSaveThreshold=save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
  //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
}
