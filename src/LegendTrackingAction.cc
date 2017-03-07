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
// $Id: LegendTrackingAction.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical/Legend/src/LegendTrackingAction.cc
/// \brief Implementation of the LegendTrackingAction class
//
//
#include "LegendAnalysis.hh"
#include "LegendTrajectory.hh"
#include "LegendTrackingAction.hh"
#include "LegendUserTrackInformation.hh"
#include "LegendDetectorConstruction.hh"
#include "LegendRecorderBase.hh"
#include "G4SystemOfUnits.hh"
#include "LegendPrimaryGeneratorAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LegendTrackingAction::LegendTrackingAction(LegendRecorderBase* r)
  : fRecorder(r) {
  
  // create directory 
  fDir = LegendAnalysis::Instance()->topDir()->mkdir("track");
  fDir->cd();
  G4cout<<" LegendTrackingAction working root directory  is  " << G4endl;  
  gDirectory->pwd();
  G4cout << " ... " << G4endl;
  G4double LowE = 2.4796*eV;//500 nm
  G4double HighE = 12.3984*eV;//100 nm
  hTrackPhotonE = new TH1F("TrackPhotonE"," photon energy in LAr",1000,LowE,HighE);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new LegendTrajectory(aTrack));

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(new LegendUserTrackInformation);
  /*  const G4VProcess* creator = aTrack->GetCreatorProcess();
  if(creator)
    G4cout<<creator->GetProcessName()<<G4endl;
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LegendTrackingAction::PostUserTrackingAction(const G4Track* aTrack){
  LegendTrajectory* trajectory=(LegendTrajectory*)fpTrackingManager->GimmeTrajectory();
  LegendUserTrackInformation* trackInformation=(LegendUserTrackInformation*)aTrack->GetUserInformation();

  //Lets choose to draw only the photons that hit the sphere and a pmt
  /*if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){

    const G4VProcess* creator=aTrack->GetCreatorProcess();
    if(creator && creator->GetProcessName()=="OpWLS"){
      trajectory->WLS();
      trajectory->SetDrawTrajectory(true);
    }

    if(LegendDetectorConstruction::GetSphereOn()){
      if((trackInformation->GetTrackStatus()&hitPMT)&&
         (trackInformation->GetTrackStatus()&hitSphere)){
        trajectory->SetDrawTrajectory(true);
      }
    }
    else{
      if(trackInformation->GetTrackStatus()&hitPMT)
        trajectory->SetDrawTrajectory(true);
    }
  }
  else //draw all other trajectories*/
//    trajectory->SetDrawTrajectory(true);

 // if(trackInformation->GetForceDrawTrajectory())
   // trajectory->SetDrawTrajectory(true);
  
  const G4VProcess* creator=aTrack->GetCreatorProcess();
  
  if(aTrack->GetDefinition() ==G4OpticalPhoton::OpticalPhotonDefinition()){
    if(creator && creator->GetProcessName()!= "Scintillation") 
      G4cout<<"LegendTrackingAction.cc:: "<<"Optical Photon Creation Process that is not Scintillation is:: "<<creator->GetProcessName()<<G4endl;
    if(creator && creator->GetProcessName() ==  "Scintillation"){
      G4double KE = aTrack->GetTotalEnergy();//Returns energy in MeV
      //G4double KE = aTrack->GetKineticEnergy();//Returns energy in MeV
      //LAr_Spectrum->Fill(KE);
      if(trackInformation->GetTrackStatus()&absorbed){
        hTrackPhotonE->Fill(KE);
		    trajectory->SetDrawTrajectory(true);
      }
    }
	}
  if(fRecorder)fRecorder->RecordTrack(aTrack);
}
