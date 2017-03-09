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
    
    //LAr Construction Fucntions
    void ArgonOpticalProperties();
    G4double LArEpsilon(const G4double lambda);
    G4double LArRefIndex(const G4double lambda);
    G4double LArRayLength(const G4double lambda,const G4double temp);
    G4double ArScintillationSpectrum(const G4double kk);

    //void Skin_Builder();//this was a dumb function and if you liked it, you are dumb too
    // LegendDetectorConstruction constant LambdaE =1.23984e-09
    G4double TPBEmissionSpectrum(G4double energy) { 
      G4double wavelength = LambdaE/energy/nm;
      G4double eff=0;
      if(wavelength>350.0 && wavelength < 650.0) eff =fTPBspec->Eval(wavelength);
      //if (eff < 0.2) eff = 0.2;
      //G4cout<<" TPBemission energy =" << energy << " nm " << nm  << " << wavelength (nm)  " << wavelength << " eff= "  << eff << G4endl;
      //MGLog(routine) << "Eval ("<< targetf/nm<< ")yielded a value of " << eff << endlog;
      return eff;
    }

    G4double VM2000EmissionSpectrum(G4double energy) { 
      G4double wavelength = LambdaE/energy/nm;
      G4double eff=0;
      if(wavelength>350.0 && wavelength < 650.0) eff =fVM2000spec->Eval(wavelength);
      return eff;
    }

    G4double getWavelength(G4double energy) {
        return LambdaE/energy/nm;
    }
    TDirectory *fDir;
    TH1F *hDetecWLSPhotonE;
    TH1F* hDetecWLSPhotonWavelength;

  /// Define some colors that will be used later
  //visualization attributes
  /*G4Colour lgreen (0.0,  0.4, 0.0) ;
  G4Colour lblue  (0.0,  0.0, 0.4) ;
  G4Colour llblue (0.,  0.0, 0.04) ;
  G4Colour blue_gray  (175/255. ,157/255. ,189/255. ) ;
  G4Colour red    (1,  0.0, 0.0);
  G4Colour lred   (0.4,  0.0, 0.0);
  G4Colour light_gray(214./255.,214./255.,214./255.);
  G4Colour dark_gray(135./255.,135./255.,135./255.);
*/
//  Not sure why colours won't work. Something wrong in the declaration  G4Colour lgreen (0.0,  0.4, 0.0) ;

  private:
    static const G4double LambdaE;
    TFile *tpbFile;
    TFile *fVM2000File;
    TGraph *fTPBspec;
    TGraph *fVM2000spec;
    LegendDetectorMessenger* detectorMessenger;  // pointer to the Messenger
		G4bool checkOverlaps;
		G4String detector_type;
		G4String innerVessel_FillMaterial;
		//#include "fTPBspec.ihh"
		//Basic volumes
		G4Tubs* solid_Pmt;
		G4Tubs* solid_Photocath;		
    G4Box* solid_World;
   	G4Box* solid_Rock;
    G4Box* solid_Lab;
    G4Tubs* solid_DetGeCrystal;
    G4Polycone* solid_innerVessel; 
    //G4Polycone* solid_fillGas; 
    G4Tubs* solid_fillGas;

    //G4Materials...G4Elements
    //G4Material* fPstyrene;
    //G4Element* fC;
    //G4Element* fH;
    //G4Material* fGlass;
    //G4Material* fAl;
    //G4Element* fN;
    //G4Element* fO;
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
    G4LogicalVolume* logical_wls;
    G4LogicalVolume* logical_VM2000Cylinder;
    G4LogicalVolume* logical_PMTGlassWLS;

    //Physical Volume: Get Physical
    G4VPhysicalVolume* physical_Rock;
    G4VPhysicalVolume* physical_World;
    G4VPhysicalVolume* physical_innerVessel;
    G4VPhysicalVolume* physical_PMT;
    G4VPhysicalVolume* physical_Photocath;
    G4VPhysicalVolume* physical_ScintSlab;
    G4VPhysicalVolume* physical_fillGas;
    G4VPhysicalVolume* physical_wls;
    
    //Surface Objects
    G4LogicalSkinSurface *  skin_copper;
    G4LogicalSkinSurface *  skin_photocath;
    G4LogicalSkinSurface* fSkin_WLS;
    G4OpticalSurface* fWLSoptSurf;
    G4OpticalSurface* fPMTGlassOptSurface;
    G4LogicalBorderSurface * wls_LogicalInnerSuface ;
    G4LogicalBorderSurface * wls_LogicalOuterSurface ;
        //Sensitive Detectors
		G4Cache<LegendScintSD*> Scint_SD;
    G4Cache<LegendPMTSD*> Pmt_SD ;

		//Main volume class object->idk what these are called :)
		LegendDetectorConstruction * fMainVolume;

    G4MaterialPropertiesTable* fMPTPStyrene;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

