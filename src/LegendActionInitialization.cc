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
// $Id: LegendActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file LegendActionInitialization.cc
/// \brief Implementation of the LegendActionInitialization class

#include "LegendActionInitialization.hh"

#include "LegendPrimaryGeneratorAction.hh"

#include "LegendRunAction.hh"
#include "LegendEventAction.hh"
#include "LegendTrackingAction.hh"
#include "LegendSteppingAction.hh"
#include "LegendStackingAction.hh"
#include "LegendSteppingVerbose.hh"

#include "LegendRecorderBase.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendActionInitialization::LegendActionInitialization(LegendRecorderBase* recorder)
 : G4VUserActionInitialization(), fRecorder(recorder)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendActionInitialization::~LegendActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendActionInitialization::BuildForMaster() const
{
  SetUserAction(new LegendRunAction(fRecorder));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendActionInitialization::Build() const
{
  SetUserAction(new LegendPrimaryGeneratorAction());//particle gun set to shoot a 511 keV photon

  SetUserAction(new LegendStackingAction());//classifies tracks to be scint or cerenkov and counts how many of each there are
  SetUserAction(new LegendRunAction(fRecorder));//Jbaited
  SetUserAction(new LegendEventAction(fRecorder));//figures out stats on event
  SetUserAction(new LegendTrackingAction(fRecorder));//Draws Scintillation photons above a certain Energy Threshold
  SetUserAction(new LegendSteppingAction(fRecorder));//Figures out boundry processes
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose* LegendActionInitialization::InitializeSteppingVerbose() const
{
  return new LegendSteppingVerbose();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
