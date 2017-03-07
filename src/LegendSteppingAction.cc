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
// $Id: LegendSteppingAction.cc 73915 2013-09-17 07:32:26Z gcosmo $
//
/// \file optical/Legend/src/LegendSteppingAction.cc
/// \brief Implementation of the LegendSteppingAction class
//
//
#include "LegendSteppingAction.hh"
#include "LegendEventAction.hh"
#include "LegendTrackingAction.hh"
#include "LegendTrajectory.hh"
#include "LegendPMTSD.hh"
#include "LegendUserTrackInformation.hh"
#include "LegendUserEventInformation.hh"
#include "LegendSteppingMessenger.hh"
#include "LegendRecorderBase.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendSteppingAction::LegendSteppingAction(LegendRecorderBase* r)
  : fRecorder(r),fOneStepPrimaries(false)
{
  fSteppingMessenger = new LegendSteppingMessenger(this);

  fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendSteppingAction::~LegendSteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendSteppingAction::UserSteppingAction(const G4Step * theStep){

  G4Track* theTrack = theStep->GetTrack();

  if ( theTrack->GetCurrentStepNumber() == 1 ) fExpectedNextStatus = Undefined;
 
  LegendUserTrackInformation* trackInformation = (LegendUserTrackInformation*)theTrack->GetUserInformation();

  LegendUserEventInformation* eventInformation = (LegendUserEventInformation*)G4EventManager::GetEventManager()
                                                  ->GetConstCurrentEvent()->GetUserInformation();

  G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  static G4ThreadLocal G4OpBoundaryProcess* boundary = NULL;

  //find the boundary process only once
  if(!boundary)
  {
    G4ProcessManager* pm = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i = 0; i < nprocesses; i++)
    {
      if((*pv)[i]->GetProcessName()=="OpBoundary" )
      {
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        G4cout<<boundary<<" = what is the Boundry status"<<G4endl;
        break;
      }
    }
  }

  if(theTrack->GetParentID()==0)
  {
    //This is a primary track
    G4TrackVector* fSecondary = fpSteppingManager->GetfSecondary();
    G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt()
                        + fpSteppingManager->GetfN2ndariesAlongStepDoIt()
                        + fpSteppingManager->GetfN2ndariesPostStepDoIt();

    //If we havent already found the conversion position and there were
    //secondaries generated, then search for it
    if(!eventInformation->IsConvPosSet() && tN2ndariesTot>0 ){
      for(size_t lp1=(*fSecondary).size()-tN2ndariesTot; lp1<(*fSecondary).size(); lp1++)
      {
        const G4VProcess* creator=(*fSecondary)[lp1]->GetCreatorProcess();
        if(creator)
        {
          G4String creatorName=creator->GetProcessName();
          if(creatorName=="phot"||creatorName=="compt"||creatorName=="conv")
          {
            //since this is happening before the secondary is being tracked
            //the Vertex position has not been set yet(set in initial step)
            eventInformation->SetConvPos((*fSecondary)[lp1]->GetPosition());
          }
        }
      }
    }
    //I wonder what this does?
    if(fOneStepPrimaries&&thePrePV->GetName()=="scintillator") theTrack->SetTrackStatus(fStopAndKill);
  }

  if(!thePostPV)//out of the world
  {
    fExpectedNextStatus=Undefined;
    return;
  }

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())
  {
    //Optical photon only

    if(thePrePV->GetName()=="phy_ScintSlab")
      //force drawing of photons in WLS slab
      trackInformation->SetForceDrawTrajectory(true);
    else 
    //Kill photons entering expHall from something other than Slab
    if(thePostPV->GetName()=="phy_Rock")//"expHall") 
    {
      G4cout<<thePostPoint->GetProcessDefinedStep()->GetProcessName()<<"did they kill the process at the boundy?"<<G4endl;
      theTrack->SetTrackStatus(fStopAndKill);
    }

    //Was the photon absorbed by the absorption process
    if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpAbsorption")
    {
      eventInformation->IncAbsorption();
      trackInformation->AddTrackStatusFlag(absorbed);
    }

    boundaryStatus=boundary->GetStatus();

    //Check to see if the partcile was actually at a boundary
    //Otherwise the boundary status may not be valid
    //Prior to Geant4.6.0-p1 this would not have been enough to check
    if(thePostPoint->GetStepStatus()==fGeomBoundary)
    {
      if(fExpectedNextStatus==StepTooSmall)
      {
        if(boundaryStatus!=StepTooSmall)
        {
          G4ExceptionDescription ed;
          ed << "LegendSteppingAction::UserSteppingAction(): "
                << "No reallocation step after reflection!"
                << G4endl;
          G4Exception("LegendSteppingAction::UserSteppingAction()", "LegendExpl01",
          FatalException,ed,
          "Something is wrong with the surface normal or geometry");
        }
      }
      fExpectedNextStatus=Undefined;
      switch(boundaryStatus){
      case Absorption:
        G4cout<<"did a photon get Absorbed?"<<G4endl;
        trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
        eventInformation->IncBoundaryAbsorption();
        break;
      case Detection: //Note, this assumes that the volume causing detection
                      //is the photocathode because it is the only one with
                      //non-zero efficiency
        {
          //Triger sensitive detector manually since photon is
          //absorbed but status was Detection
          G4SDManager* SDman = G4SDManager::GetSDMpointer();
          G4String sdName="/LegendDet/pmtSD";
          LegendPMTSD* pmtSD = (LegendPMTSD*)SDman->FindSensitiveDetector(sdName);
          if(pmtSD) pmtSD->ProcessHits_constStep(theStep,NULL);
          trackInformation->AddTrackStatusFlag(hitPMT);
          break;
        }
      case FresnelReflection:
      case TotalInternalReflection:
      case LambertianReflection:
      case LobeReflection:
      case SpikeReflection:
      case BackScattering:
        trackInformation->IncReflections();
        fExpectedNextStatus=StepTooSmall;
        break;
      default:
        break;
      }
      //if(thePostPV->GetName()=="sphere") trackInformation->AddTrackStatusFlag(hitSphere);
    }
   
  }

  if(fRecorder)fRecorder->RecordStep(theStep);
}
