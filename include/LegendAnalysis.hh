#ifndef LegendAnalysis_h
#define LegendAnalysis_h 1

#include "globals.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsCollection.hh"
#include "LegendScintSDHit.hh"
#include "LegendPMTSDHit.hh"

//#include "g4root.hh"
#include "TFile.h"
#include "TH1F.h"
#include "LTEvent.hxx"
// singleton class for root file handling
// M.G. 
// .. is it multi-thread safe?
class LegendAnalysis
{
  private:
    LegendAnalysis() { Initialize(); };
    void Initialize();
    TFile *fFile;
    TTree *fTree;
    LTEvent *fEvent;
    static LegendAnalysis* fLegendAnalysis; 
    // Disabled (not implemented) copy constructor and asignment.
    LegendAnalysis(const LegendAnalysis&);
    LegendAnalysis& operator=(const LegendAnalysis&);
    int fScintCollID;  // store collection IDs
    int fPMTCollID;
    TH1F *hWOptical;
    TH1F *hEElectron;
    TH1F *hEGamma;
  
 
  public:
    ~LegendAnalysis() {
      printf(" number of entries in LegendAnalysis tree is %i \n",(int) fTree->GetEntries() );
      fFile->ls();
      fFile->Write();
      fFile->Close();
    }
    static LegendAnalysis* Instance();
    
    TDirectory *topDir() { return (TDirectory* ) fFile;}
    TTree *getTree() { return fTree; }
    void anaEvent(const G4Event* anEvent);
    void anaTrajectories(G4TrajectoryContainer* trajectoryContainer);
    void setScintCollID(int id) { fScintCollID=id; }
    void setPMTCollID(int id) { fPMTCollID=id; }
}
      
    
;
#endif
