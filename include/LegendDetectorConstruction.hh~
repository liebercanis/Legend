///////////////////////////////////////////////////////////////////////////
// This code implementation is the intellectual property of the
// ton-scale 0vbb in Germanium collaboration. It is based on Geant4, an
// intellectual property of the RD44 GEANT4 collaboration.
//
// *********************
//
// Neither the authors of this software system, nor their employing
// institutes, nor the agencies providing financial support for this
// work make any representation or warranty, express or implied,
// regarding this software system or assume any liability for its use.
// By copying, distributing or modifying the Program (or any work based
// on the Program) you indicate your acceptance of this statement,
// and all its terms.
/// \file LegendDetectorConstruction.hh
/// \brief Definition of the LegendDetectorConstruction class

#ifndef LegendDetectorConstruction_h
#define LegendDetectorConstruction_h 1

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4Polycone.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4PhysicalConstants.hh"
#include "G4Polyhedra.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Torus.hh"
#include "G4UnionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4NistManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"

#include "LegendDetectorMessenger.hh"
#include "LegendScintSD.hh"
#include "LegendPMTSD.hh"
#include "LegendAnalysis.hh"

// -- ROOT include
#include "TGraph.h"
#include "TFile.h"


class G4VPhysicalVolume;
//class DetectorMessenger;

/// Detector construction class to define materials and geometry.

class LegendDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    LegendDetectorConstruction();
    virtual ~LegendDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

		void UpdateGeometry();
		void SetOverlapsCheck(G4bool);
		void SetShieldStyle(G4String);
		void SetFillGas(G4String);
		void ConstructSDandField();

		G4LogicalVolume* GetLogPhotoCath() {return logical_Photocath;}
		G4LogicalVolume* GetLogScint()     {return logical_fillGas;}
    
    void PlacePMT(G4double x,G4double y,G4double z,double top_or_bot,int num);    
    
    //LAr Construction Fucntions
    void ArgonOpticalProperties();
    G4double LArEpsilon(const G4double lambda);
    G4double LArRefIndex(const G4double lambda);
    G4double LArRayLength(const G4double lambda,const G4double temp);
    G4double ArScintillationSpectrum(const G4double kk);

    G4double TPBEmissionSpectrum(G4double energy) { 
      G4double wavelength = LambdaE/energy/nm;
      G4double eff=0;
      if(wavelength>350.0 && wavelength < 650.0) eff =fTPBspec->Eval(wavelength);
      return eff;
    }

    TDirectory *fDir;
    TH1F *hDetecWLSPhotonE;
    TH1F* hDetecWLSPhotonWavelength;

  private:
    /// PMTs + WLS coating
    G4double innerR_cryo;
    G4double thickness_WLS;  
    G4double PMTHousing_Thickness; 
    G4double PMTGlass_Thickness;
    G4double PMT_Radius;
    G4double PMT_Height;
    G4double startAngle_pmt;
    G4double spanningAngle_pmt;

    static const G4double LambdaE;
    int numPMT = 0;
    std::vector<G4LogicalVolume*> pmtVector;
    std::vector<G4String> pmtName;
    std::vector<G4int> pmtInstance;
    TFile *tpbFile;
    TFile *fVM2000File;
    TGraph *fTPBspec;
    TGraph *fVM2000spec;
    LegendDetectorMessenger* detectorMessenger;  // pointer to the Messenger
		G4bool checkOverlaps;
		G4String detector_type;
		G4String innerVessel_FillMaterial;
		
    //Basic volumes
		G4Tubs* solid_Pmt;
		G4Tubs* solid_Photocath;		
    G4Box* solid_World;
   	G4Box* solid_Rock;
    G4Box* solid_Lab;
    G4Tubs* solid_DetGeCrystal;
    G4Polycone* solid_innerVessel; 
    G4Tubs* solid_fillGas;
    
    //Materials...most are found in materials files
    G4Material* mat_fillGas;
    G4Material*  fTPB;
    G4MaterialPropertiesTable *tpbTable;
    G4MaterialPropertiesTable *fPMTGlassOptTable;
    G4Material* mat_ArLiq;
    G4Material* mat_ArCold;
    G4Material* mat_NCold;
    G4Material* mat_NLiq;

    //Logical Volume  
    G4LogicalVolume* logical_fillGas;
    G4LogicalVolume* logical_Pmt;
 		G4LogicalVolume* logical_Photocath;
    G4LogicalVolume* logical_ScintSlab;
 		G4LogicalVolume* logical_copperShield;
    G4LogicalVolume* logical_World;
    G4LogicalVolume* logical_Rock;
    G4LogicalVolume* logical_DetGeCrystal;
    G4LogicalVolume* logical_innerVessel;
    G4LogicalVolume* logical_wlsCylinder;
    G4LogicalVolume* logical_wlsDisk;
    G4LogicalVolume* logical_VM2000Cylinder;
    G4LogicalVolume* logical_PMTGlassWLS;
    G4LogicalVolume* logical_PMTGlass; //use this for Sensitive Detector
    G4LogicalVolume* logical_PMTHousing;

    //Physical Volume: Get Physical
    G4VPhysicalVolume* physical_Rock;
    G4VPhysicalVolume* physical_World;
    G4VPhysicalVolume* physical_innerVessel;
    G4VPhysicalVolume* physical_PMT;
    G4VPhysicalVolume* physical_Photocath;
    G4VPhysicalVolume* physical_ScintSlab;
    G4VPhysicalVolume* physical_fillGas;
    G4VPhysicalVolume* physical_wlsCylinder;
    G4VPhysicalVolume* physical_wlsDiskTop;
    G4VPhysicalVolume* physical_wlsDiskBottom;
    
    //Surface Objects
    G4LogicalSkinSurface *  skin_copper;
    G4LogicalSkinSurface *  skin_photocath;
    G4LogicalSkinSurface* fSkin_WLS;
    G4OpticalSurface* fWLSoptSurf;
    G4OpticalSurface* fPMTGlassOptSurface;
    G4LogicalBorderSurface * wlsCylinder_LogicalInnerSuface ;
    G4LogicalBorderSurface * wlsCylinder_LogicalOuterSurface ;
    G4LogicalBorderSurface * wlsDiskTop_LogicalInnerSuface ;
    G4LogicalBorderSurface * wlsDiskTop_LogicalOuterSurface ;
    G4LogicalBorderSurface * wlsDiskBottom_LogicalInnerSuface ;
    G4LogicalBorderSurface * wlsDiskBottom_LogicalOuterSurface ;

    G4MaterialPropertiesTable* fMPTPStyrene;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

