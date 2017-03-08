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
#include "LegendAnalysis.hh"

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

  // create directory 
  fDir = LegendAnalysis::Instance()->topDir()->mkdir("step");
  fDir->cd();
  G4cout<<" LegendStepAction working root directory  is  " << G4endl;  
  gDirectory->pwd();
  G4cout << " ... " << G4endl;
  G4double LowE = 1.7712*eV;//700 nm
  G4double HighE = 12.3984*eV;//100 nm
  hWLSPhotonE = new TH1F("StepWLSPhotonE"," photon energy from WLS",1000,LowE,HighE);

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
      //if( !((*pv)[i]->GetProcessName()=="OpBoundary") ) G4cout<<"Processes that are not OpBoundary :: "<<(*pv)[i]->GetProcessName()<<G4endl;
      if((*pv)[i]->GetProcessName()=="OpBoundary" )
      {
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        G4cout<<"The Boundry status is ::"<<boundary->GetStatus()<<"\n\t0:Undefined\n\t1::Transmission\n\t2::FresnelRefraction\n\t3::FresnelReflection\n\t4::TotalInternalReflection\n\t5::LambertianReflection\n\t6::LobeReflection\n\t7::SpikeReflection\n\t8::BackScattering\n\t9:Absorption \n\t10:Detectoin \n\t11:NotAtBoundry \n\t12::SameMaterial\n\t13::StepTooSmall\n\t14::NoRINDEX"<<G4endl;
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
          //G4cout<<"CreatorName :: "<<creatorName<<G4endl;
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
    //Orginally it was scint... with  lower case s, but should be CAP Scint...
    if(fOneStepPrimaries&&thePrePV->GetName()=="Scintillator") theTrack->SetTrackStatus(fStopAndKill);
  }

  if(!thePostPV)//out of the world
  {
    G4cout<<"Primary Vertex is out of this world \n Ending Stepping Action!"<<G4endl;
    fExpectedNextStatus=Undefined;
    return;
  }

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())
  {
    //Optical photon only
    if(thePrePV->GetName()!="phys_WLSCylinderPhysical" && thePrePV->GetName()!="phy_fillGas"){
 //     G4cout<<"LegendSteppingAction:: The Pre PV Name:: "<<thePrePV->GetName()<<G4endl;
    }
    if(thePrePV->GetName()=="phys_WLSCylinderPhysical"){
      //force drawing of photons in WLS slab
      //G4cout<<"A photon hit the WLS slab names phy_ScintSlab"<<G4endl;
      trackInformation->SetForceDrawTrajectory(true);
      G4double KE = theTrack->GetKineticEnergy();
      hWLSPhotonE->Fill(KE);
    }
    else if(thePostPV->GetName()=="phys_WLSCylinderPhysical"){
    }
    //Kill photons entering expHall from something other than Slab
    if(thePostPV->GetName()=="phy_World")//"expHall") 
    {
//      G4cout<<thePostPoint->GetProcessDefinedStep()->GetProcessName()<<"did they kill the process at the boundy?"<<G4endl;
      theTrack->SetTrackStatus(fStopAndKill);
      eventInformation->IncBoundaryAbsorption();
    }

    //Was the photon absorbed by the absorption process
    if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpAbsorption")
    {
      eventInformation->IncAbsorption();
      trackInformation->AddTrackStatusFlag(absorbed);
    }
    if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpBoundry"){
      G4cout<<"OpBoundry is foundry"<<G4endl;
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
          G4cout<<"LegendSteppinAction:: thePrePV of Process is :: "<< thePrePV->GetName()<<G4endl;
          G4cout<<"LegendSteppinAction:: thePostPV of Process is :: "<< thePostPV->GetName()<<G4endl;
          
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
        //G4cout<<"LegendSteppingAction.cc:: "<<thePostPoint->GetProcessDefinedStep()->GetProcessName()<<"::Where the Photon was Absorbed"<<G4endl;
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
      //added by Neil
      case NotAtBoundary:
      default:
        break;
      }
      //if(thePostPV->GetName()=="sphere") trackInformation->AddTrackStatusFlag(hitSphere);
    }   
  }//end of if(thePostPoint->GetStepStatus()==fGeomBoundary)
 // else G4cout<<"Particle type that is not optical is :: "<<particleType->GetParticleName()<<G4endl;
 // This statement shows that everything that is not optical is either a e- or a gamma (which is different from optical photon)

  if(fRecorder)fRecorder->RecordStep(theStep);
}
