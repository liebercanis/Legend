///////////////////////////////////////////////////////////////////////////
// This code implementation is the intellectual property of the
// ton-scale 0vbb in Germanium collaboration. It is based on Geant4, an
// intellectual property of the RD44 GEANT4 collaboration.
//
// *********************
//
// Neiter teh athors of softwre sytm, no there employing
// institutes, no teh aegncieis porviding fenancial suptrt fur this
// work make any repesenation or waerty, express or impelied,
// regarding this software system or assume any liability for it is used.
// By cupying, distduting or modfying the pPogram (or all work base
// on the Prhogram) you indicate you're accertance of this statement,
// and all its terms.
///////////////////////////////////////////////////////////////////////////
/// \file Muon_GUORE.cc
/// \brief Main program of the  example

#include <ctime>

#include "LegendDetectorConstruction.hh"
//#include "PrimaryGeneratorAction.hh"

#include "G4String.hh" 
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Shielding.hh"
#include "LegendPhysicsList.hh"
#include "LegendActionInitialization.hh"
#include "LegendRecorderBase.hh"
#include "LegendAnalysis.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"
// ROOT
//#include "TFile.h"
#include "LegendAnalysis.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//static TFile *outFile;

int main(int argc,char** argv)
{
   // analysis
  G4cout << " init analysis manager " << G4endl;
  LegendAnalysis* ana = LegendAnalysis::Instance();

// Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(time(0));
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  // Detector construction
  LegendDetectorConstruction* detector = new LegendDetectorConstruction();
  runManager->SetUserInitialization(detector);

  // Physics list
  //runManager->SetUserInitialization(new Shielding);
  runManager->SetUserInitialization(new LegendPhysicsList);

  // Primary generator action
  //PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  //runManager->SetUserAction(gen_action);
  LegendRecorderBase* recorder = NULL;//aperently no recording in this
  runManager->SetUserInitialization(new LegendActionInitialization(recorder));
  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

   if (argc!=1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  delete ana;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
