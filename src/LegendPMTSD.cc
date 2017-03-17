//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                            MaGe Simulation                                //
//                                                                           //
//      This code implementation is the intellectual property of the         //
//      MAJORANA and Gerda Collaborations. It is based on Geant4, an         //
//      intellectual property of the RD44 GEANT4 collaboration.              //
//                                                                           //
//                        *********************                              //
//                                                                           //
//    Neither the authors of this software system, nor their employing       //
//    institutes, nor the agencies providing financial support for this      //
//    work  make  any representation or  warranty, express or implied,       //
//    regarding this software system or assume any liability for its use.    //
//    By copying, distributing or modifying the Program (or any work based   //
//    on on the Program) you indicate your acceptance of this statement,     //
//    and all its terms.                                                     //
//                                                                           //
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//---------------------------------------------------------------------------//
//                                                          
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 * Original desing by Alexander Klimenko
 */
// 
//---------------------------------------------------------------------------//
/**
 *
 * AUTHOR:  Markus Knapp
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: 
 * 
 * REVISION:
 *
 */
//---------------------------------------------------------------------------//
//



#include "LegendPMTSD.hh"
#include "LegendPMTSDHit.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "Randomize.hh"

//#include "io/MGLogger.hh"//Gerda log maker...will flush later

G4ThreeVector pos;

LegendPMTSD::LegendPMTSD(G4String name, G4int nCells, G4String colName)
:G4VSensitiveDetector(name),numberOfCells(nCells),HCID(-1)
{
  G4String HCname;
  collectionName.insert(HCname=colName);
  CellID = new int[numberOfCells];
 // MGLog(trace) << " ==--> "<< colName << "  "<< name<<"  LegendPMTSD  nCells= "<<nCells <<" SD name " << name<<"  \n" << endlog;

}

LegendPMTSD::~LegendPMTSD()
{
  delete [] CellID;
}

void LegendPMTSD::Initialize(G4HCofThisEvent* /*HCE*/)
{
//	MGLog(trace) << " ==--> LegendPMTSD  collection initialized \n" << endlog;
  PMTCollection = new LegendPMTSDHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 

  for(int j=0;j<numberOfCells;j++)   
    { 
      CellID[j] = -1;
    }
  TimeInit=-10.0;

}
G4bool LegendPMTSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  return ProcessHits_constStep(aStep,NULL);
}

G4bool LegendPMTSD::ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory* )
{

  G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
  G4String particleName = particleType->GetParticleName();
  G4double edep = aStep->GetTotalEnergyDeposit();
  //  G4double time = aStep->GetPostStepPoint()->GetGlobalTime();   //measured in nanoseconds;
  //  G4double time = aStep->GetTrack()->GetGlobalTime();
  //  G4cout << "testpoint " << time <<  "\n";

  if(edep<=0. || (particleName != "opticalphoton")) return false;

  const G4VPhysicalVolume* physVol = aStep->GetPostStepPoint()->GetPhysicalVolume();
  G4int copyID = physVol->GetCopyNo();

  // make new pmtSDHit 
  LegendPMTSDHit* pmtHit = new LegendPMTSDHit(physVol->GetLogicalVolume());
  G4RotationMatrix rotM;
  if(physVol->GetObjectRotation()) rotM = *(physVol->GetObjectRotation());
  pmtHit->SetRot( rotM );
  pmtHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  pmtHit->SetTotEnergy(aStep->GetTrack()->GetTotalEnergy() );
  pmtHit->SetHittime( aStep->GetTrack()->GetGlobalTime() );
  pmtHit->SetParticleDirection((aStep->GetPreStepPoint())->GetMomentumDirection());
  
  if(CellID[copyID]==-1) {
    pmtHit->SetEdep( edep );
    pmtHit->Set0Photon(); // sets nPhoton=1 ... Hits number for initial time
    pmtHit->SetNdet( copyID );
    pmtHit->SetParticleDirection((aStep->GetPreStepPoint())->GetMomentumDirection());
    int icell = PMTCollection->insert( pmtHit );
    CellID[copyID] = icell - 1;
  }  else {
    pmtHit->IncrementPhotons();
    pmtHit->AddEdep(edep);
    pmtHit->SetNdet(copyID+1);
    PMTCollection->insert( pmtHit );
  }
  return true;
}

void LegendPMTSD::EndOfEvent(G4HCofThisEvent*HCE)
{
  if(HCID<0)
    {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }

  HCE->AddHitsCollection( HCID, PMTCollection );
}

void LegendPMTSD::clear()
{

} 

void LegendPMTSD::DrawAll()
{
  
} 

void LegendPMTSD::PrintAll()
{

} 




