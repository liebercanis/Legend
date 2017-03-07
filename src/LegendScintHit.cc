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
// $Id: LegendScintHit.cc 72250 2013-07-12 08:59:26Z gcosmo $
//
/// \file optical/Legend/src/LegendScintHit.cc
/// \brief Implementation of the LegendScintHit class
//
//
#include "LegendScintHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4ThreadLocal G4Allocator<LegendScintHit>* LegendScintHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendScintHit::LegendScintHit() : fEdep(0.), fPos(0.), fPhysVol(0) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendScintHit::LegendScintHit(G4VPhysicalVolume* pVol) : fPhysVol(pVol) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendScintHit::~LegendScintHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendScintHit::LegendScintHit(const LegendScintHit &right) : G4VHit()
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fPhysVol = right.fPhysVol;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const LegendScintHit& LegendScintHit::operator=(const LegendScintHit &right){
  fEdep = right.fEdep;
  fPos = right.fPos;
  fPhysVol = right.fPhysVol;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int LegendScintHit::operator==(const LegendScintHit&) const{
  return false;
  //returns false because there currently isnt need to check for equality yet
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendScintHit::Draw() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendScintHit::Print() {}
