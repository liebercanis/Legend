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
// $Id: LegendTrackingAction.hh 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical/Legend/include/LegendTrackingAction.hh
/// \brief Definition of the LegendTrackingAction class
//
//
#ifndef LegendTrackingAction_h
#define LegendTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "TH1F.h"
#include "TDirectory.h"
class LegendRecorderBase;

class LegendTrackingAction : public G4UserTrackingAction {

  public:

    LegendTrackingAction(LegendRecorderBase*);
    virtual ~LegendTrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
    TDirectory *fDir;
    TH1F *hTrackPhotonE;
 
  //TH1F *LAr_Spectrum = new TH1F("Scintillation from LAr"," counts vs photon energy in LAr",N,Nrg[0],Nrg[N-1]);

  private:
  
 
  LegendRecorderBase* fRecorder;

};

#endif
