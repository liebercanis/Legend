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
#include "LegendScintSD.hh"
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
#include "G4VProcess.hh"

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
  
  if(!thePostPV){//out of the world
    G4cout<<"LegendSteppingAction::Primary Vertex is out of this world \n\t Ending Stepping Action!"<<G4endl;
    fExpectedNextStatus=Undefined;
    return;
  }
  
  //This is a primary track 
  // did we miss any secondaries from the primary track?
  if(theTrack->GetParentID()==0){
    G4TrackVector* fSecondary = fpSteppingManager->GetfSecondary();
    G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt()
                        + fpSteppingManager->GetfN2ndariesAlongStepDoIt()
                        + fpSteppingManager->GetfN2ndariesPostStepDoIt();

    //If we havent already found the conversion position and there were
    //Loop over all 2ndaries that have not been found with N2ndariesTot
    if(!eventInformation->IsConvPosSet() && tN2ndariesTot>0 ){
      for(size_t lp1=(*fSecondary).size()-tN2ndariesTot; lp1<(*fSecondary).size(); lp1++){
        const G4VProcess* creator=(*fSecondary)[lp1]->GetCreatorProcess();
        if(creator){
          G4String creatorName=creator->GetProcessName();
          //Added Scint to list -Neil
          if(creatorName=="phot"||creatorName=="compt"||creatorName=="conv"||creatorName=="Scintillation"){
            //since this is happening before the secondary is being tracked
            //the Vertex position has not been set yet(set in initial step)
            //so set Conversion Position
            eventInformation->SetConvPos((*fSecondary)[lp1]->GetPosition());
          } else G4cout << " LegendSteppingActoin unknown creatorName " << creatorName << G4endl;
        }
      }
    }
  }


  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  static G4ThreadLocal G4OpBoundaryProcess* boundary = NULL;

  //find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i = 0; i < nprocesses; i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary" ){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }

  //Used to find othe non optical processes
  const G4VProcess * process = theTrack->GetCreatorProcess();
  
  G4String processName;
  if(process) processName = process->GetProcessName();

  G4ParticleDefinition* particleType = theTrack->GetDefinition();

  //Optical Photons
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
    
    //Need local definition for ScintSDHit processing
    G4Step* step = const_cast<G4Step*>(theStep);

    //Kill photons exiting cryostat
    if(thePostPV->GetName()=="phy_World"){
      theTrack->SetTrackStatus(fStopAndKill);
      eventInformation->IncPhotonCount_Escape();
      return;
    }

    //The photon was absorbed at another place other than a boundry
    if(thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpAbsorption"){
      eventInformation->IncAbsorption();
      trackInformation->AddTrackStatusFlag(absorbed);
      //if the photon was absorbed in LAr ProcessHit
      if(thePrePV->GetName()=="phy_fillGas"){
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4String sdName="ScintSD";
        LegendScintSD* ScintSD = (LegendScintSD*)SDman->FindSensitiveDetector(sdName);
        if(ScintSD){ 
          ScintSD->ProcessHits(step,NULL);
        }
        trackInformation->AddTrackStatusFlag(absorbedLAr);
      }
    }
    
    boundaryStatus=boundary->GetStatus();
    
    //Check to see if the partcile was actually at a boundary
    //Otherwise the boundary status may not be valid
    //Prior to Geant4.6.0-p1 this would not have been enough to check
    /* enum G4OpBoundaryProcessStatus {  Undefined,
                                  Transmission, FresnelRefraction,
                                  FresnelReflection, TotalInternalReflection,
                                  LambertianReflection, LobeReflection,
                                  SpikeReflection, BackScattering,
                                  Absorption, Detection, NotAtBoundary,
                                  SameMaterial, StepTooSmall, NoRINDEX,
      .... and more in G4OpBoundaryProcess.hh
    */
    if(thePostPoint->GetStepStatus()==fGeomBoundary){
      if(fExpectedNextStatus==StepTooSmall){
        if(boundaryStatus!=StepTooSmall){
          G4cout<< "LegendSteppingAction::UserSteppingAction(): No reallocation step after reflection!"<<G4endl;          
          G4cout<<"LegendSteppinAction:: thePrePV of Process is :: "<< thePrePV->GetName()<<G4endl;
          G4cout<<"LegendSteppinAction:: thePostPV of Process is :: "<< thePostPV->GetName()<<G4endl;
          G4cout<<"\t    >>>>>>>>>> Something is wrong with the surface normal or geometry....Track is killed"<<G4endl;

          theTrack->SetTrackStatus(fStopAndKill);
        }
      }
      fExpectedNextStatus=Undefined;
      switch(boundaryStatus){
      case Absorption:
        {
          //This all Transportation
          trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
          eventInformation->IncBoundaryAbsorption();
          break;
        }
      case Detection:
        {
          //Note, this assumes that the volume causing detection
          //is the photocathode because it is the only one with
          //non-zero efficiency
          //Triger sensitive detector manually since photon is
          //absorbed but status was Detection
          G4SDManager* SDman = G4SDManager::GetSDMpointer();
          G4String sdName="PhotoCathode";//"/LegendDet/pmtSD";
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
        fExpectedNextStatus=BackScattering;//: StepTooSmall;
        break;
      //added by Neil
      case NotAtBoundary:
      default:
        break;
      }
    }  //end of if(thePostPoint->GetStepStatus()==fGeomBoundary) 
  }//end of if OpticalPhoton
  else if(processName == "phot" ){ 
    //Fill container
  }
  else if(processName == "eIoni"){
    //Fill container
  }
  else if(processName == "compt"){
    //Fill Container
  }
  else if(processName != ""){
    G4cout<<"LegendSteppingAction:: Process Name that Neil could not find is ... "<<processName<<" !!!"<<G4endl;
  }

}
