#ifndef LegendAnalysis_h
#define LegendAnalysis_h 1

//#include "g4root.hh"
#include "TFile.h"
#include "TTree.h"
#include "LTEvent.hxx"
#include "globals.hh"

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
  
  public:
    ~LegendAnalysis() {
      fFile->ls();
      fFile->Write();
      fFile->Close();
    }
    static LegendAnalysis* Instance();
    
    TDirectory *topDir() { return (TDirectory* ) fFile;}
    TTree *getTree() { return fTree; }
}
      
    
;
#endif
