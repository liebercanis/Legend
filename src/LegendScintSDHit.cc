//---------------------------------------------------------------------------//
//bb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nubb0nu//
//                                                                           //
//                                                                           //
//                         MaGe Simulation                                   //
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
// $Id: LegendScintSDHit.cc,v 1.4 2009-06-16 14:42:20 pandola Exp $ 
//      
// CLASS IMPLEMENTATION:  @CLASS_NAME@.cc
//
//---------------------------------------------------------------------------//
/**
 * SPECIAL NOTES:
 * 
 *
 */
// 
//---------------------------------------------------------------------------//
/**
 * AUTHOR:  Xiang Liu
 * CONTACT: @CONTACT@
 * FIRST SUBMISSION: @START_DATE@
 * 
 * REVISION:
 *
 * mm-dd-yyyy, What is change, Whoami
 * 12-06-2004, xiang liu, added this line to check syncmail
 * 12-06-2004, xiang liu, added this line to check syncmail again
 * 04-02-2007, Luciano, Added PhysicalVolume information
 */
//---------------------------------------------------------------------------//
//
#include "LegendScintSDHit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<LegendScintSDHit> LegendScintSDHitAllocator;

LegendScintSDHit::LegendScintSDHit()
{;}

LegendScintSDHit::~LegendScintSDHit()
{;}

LegendScintSDHit::LegendScintSDHit(const LegendScintSDHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  ekinetic = right.ekinetic;
  steplength = right.steplength;
  tracklength = right.tracklength;
  volumename = right.volumename;
  copynumber = right.copynumber;
  hittime    = right.hittime;
  trackid    = right.trackid;
  trackpdg   = right.trackpdg;
  trackparentid = right.trackparentid;
  pVolume = right.pVolume;
}

const LegendScintSDHit& LegendScintSDHit::operator=(const LegendScintSDHit &right)
{
  edep = right.edep;
  pos = right.pos;
  ekinetic = right.ekinetic;
  steplength = right.steplength;
  tracklength = right.tracklength;
  volumename = right.volumename;
  copynumber = right.copynumber;
  hittime    = right.hittime;
  trackid    = right.trackid;
  trackpdg   = right.trackpdg;
  trackparentid = right.trackparentid;
  pVolume = right.pVolume;
  return *this;
}

G4int LegendScintSDHit::operator==(const LegendScintSDHit &right) const
{
  return (this==&right) ? 1 : 0;
}


void LegendScintSDHit::Draw()
{;}


void LegendScintSDHit::Print()
{;}

