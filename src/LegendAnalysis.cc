/// M.G. started with exgps example 
// $Id: HistoManager.cc 83882 2014-09-22 11:09:30Z maire $
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "LegendAnalysis.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
LegendAnalysis* LegendAnalysis::fLegendAnalysis=NULL;

LegendAnalysis* LegendAnalysis::Instance() {
  if (! fLegendAnalysis) fLegendAnalysis = new LegendAnalysis();
  return fLegendAnalysis;
}

void LegendAnalysis::Initialize()
{
 // open new ouput file with time stamp.
  time_t tnow;
  time(&tnow);
  char chtime[80];
  sprintf(chtime,"%u",unsigned(tnow));
  G4String fFileName = G4String("RooTFiles/legend-") + G4String(chtime) + G4String(".root");
  fFile=new TFile(fFileName.data(),"RECREATE");
  G4String gmess= G4String(" ************  output file is ") + fFileName +  G4String(" ************ ");
  G4cout << gmess << G4endl;
  G4cout<<" LegendAnalysis working root directory  is  " << G4endl;  
  topDir()->cd();
  gDirectory->pwd();
  G4cout << " ... " << G4endl;
}

