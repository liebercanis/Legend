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
// $Id: LegendStackingAction.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical/Legend/src/LegendStackingAction.cc
/// \brief Implementation of the LegendStackingAction class
//
//
#include "LegendStackingAction.hh"
#include "LegendUserEventInformation.hh"
#include "LegendSteppingAction.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendStackingAction::LegendStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendStackingAction::~LegendStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
LegendStackingAction::ClassifyNewTrack(const G4Track * aTrack){
 
  LegendUserEventInformation* eventInformation = (LegendUserEventInformation*)G4EventManager::GetEventManager()
                                                  ->GetConstCurrentEvent()->GetUserInformation();
 
  //Count what process generated the optical photons
  //particle is optical photon
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
  {
//    G4cout<<"is it making here?"<<G4endl;
    // particle is secondary
    if(aTrack->GetParentID()>0)
    {
 //   G4cout<<"is it making here?"<<G4endl;
      if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation")
      {
        eventInformation->IncPhotonCount_Scint();
        //G4cout<<"Energy Deposited"<<eventInformation->GetEDep()<<G4endl;
      }
      else if(aTrack->GetCreatorProcess()->GetProcessName()=="Cerenkov")
        eventInformation->IncPhotonCount_Ceren();
     //else G4cout<<"Creator Process is ::"<<aTrack->GetCreatorProcess()->GetProcessName()<<G4endl;
    }
  }
 // else{
 // }
  return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendStackingAction::NewStage() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendStackingAction::PrepareNewEvent() {}
