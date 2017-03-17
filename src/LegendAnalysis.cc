/// M.G. started with exgps example 
// $Id: HistoManager.cc 83882 2014-09-22 11:09:30Z maire $
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "LegendAnalysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


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
  G4String fFileName = G4String("legend-") + G4String(chtime) + G4String(".root");
  fFile=new TFile(fFileName.data(),"RECREATE");
  G4String gmess= G4String(" ************  output file is ") + fFileName +  G4String(" ************ ");
  G4cout << gmess << G4endl;
  G4cout<<" LegendAnalysis working root directory  is  " << G4endl;  
  topDir()->cd();
  hWOptical = new TH1F("WOptical"," optical photons (nm) ",900,100,1000);
  hWOptical->GetYaxis()->SetTitle(" photons/nm ");
  hWOptical->GetXaxis()->SetTitle(" wavelength (nm) ");

  hEElectron = new TH1F("WElectron"," electrons ",1000,0,1000);
  hEElectron->GetYaxis()->SetTitle(" electrons/KeV ");
  hEElectron->GetXaxis()->SetTitle(" energy (KeV) ");
  
  hEGamma = new TH1F("WGamma"," gammas ",1000,0,1000);
  hEGamma->GetYaxis()->SetTitle(" gamma/KeV ");
  hEGamma->GetXaxis()->SetTitle(" energy (KeV) ");
  

  // make tree in output file
  fTree = new TTree("LTree","LTree");
  fEvent = new LTEvent();
  fTree->Branch("event",&fEvent);

  G4cout << " root top directory " << G4endl;
  gDirectory->pwd();
  topDir()->ls();
  G4cout << " ... " << G4endl;
  fScintCollID=-1;  // store collection IDs
  fPMTCollID=-1;
  

}
    
void  LegendAnalysis::anaEvent( const G4Event *anEvent)
{
  printf(" ++++++++++++++++++++++++++++++++++++++++++++++++ \n");
  printf("      LegendAnalysis:anaEvent called \n");
  printf(" ++++++++++++++++++++++++++++++++++++++++++++++++ \n");
  fEvent->clear();
  fEvent->evId = anEvent->GetEventID();
  fEvent->nPVert = anEvent->GetNumberOfPrimaryVertex();
  printf("\t anaEvent with %i primary verticies %i \n",fEvent->nPVert);
  for(int iv=0; iv < fEvent->nPVert; ++iv) {
    G4PrimaryVertex* pvi = anEvent->GetPrimaryVertex(iv);
    LTPVertex lpv;
    // fill primary vertex info
    lpv.VertexId=iv;
    lpv.Position.SetXYZT(pvi->GetX0(),pvi->GetY0(),pvi->GetZ0(),pvi->GetT0());
    //G4VUserPrimaryVertexInformation* pvinfo= pvi->GetUserInformation() 
    lpv.nParticles =pvi->GetNumberOfParticle();
    for(int ip=0; ip< lpv.nParticles; ++ip ) {
      G4PrimaryParticle* pvPart = pvi->GetPrimary(ip); 
      LTParticle part;
      part.TrackId= pvPart->GetTrackID();
      part.VertexId=iv;
      part.PDG=     pvPart->GetPDGcode();
      part.Mass=    pvPart->GetMass();
      part.Charge=  pvPart->GetCharge();
      part.KEnergy= pvPart->GetKineticEnergy();
      part.Momentum.SetPxPyPzE( pvPart->GetPx(),pvPart->GetPy(),pvPart->GetPz(),pvPart->GetTotalEnergy());
      part.print(ip);
      lpv.particle.push_back(part);
      if(iv==0&&part.TrackId==1) {
        fEvent->PDG=part.PDG;
        fEvent->ePrimary = pvPart->GetTotalEnergy();
      }
    }
    fEvent->pvertex.push_back(lpv);
  }
  anaTrajectories( anEvent->GetTrajectoryContainer());
  //
  G4HCofThisEvent* hitCollections = anEvent->GetHCofThisEvent();
  
  //Get the hit collections
  G4THitsCollection<LegendScintSDHit>* scintHC=NULL; 
  G4THitsCollection<LegendPMTSDHit>* pmtHC=NULL; 
  
  G4cout<< "\t number of hit collections is " <<  hitCollections->GetNumberOfCollections() <<G4endl;
  //names are ScintHC  PhCathodeHC
  
  for(size_t ic=0; ic<  hitCollections->GetNumberOfCollections() ; ++ic) {
    G4String cName =  hitCollections->GetHC(ic)->GetName();
    G4cout << " \t " << cName << G4endl;
    if(cName=="ScintHC")     scintHC = (G4THitsCollection<LegendScintSDHit>*)( hitCollections->GetHC(fScintCollID));
    if(cName=="PhCathodeHC") pmtHC = (G4THitsCollection<LegendPMTSDHit>*)(  hitCollections->GetHC(fPMTCollID));
  }

  if(scintHC) G4cout<< "\t scint hit collections " <<  scintHC->entries() << G4endl;
  else G4cout << " \t no scint hits " << G4endl;
  if(pmtHC) G4cout<< "\t pmt hit collections " <<  pmtHC->entries() << G4endl;
  else G4cout << " \t no  pmt hits " << G4endl;

  // pmt hits
  if(pmtHC) {
    G4int pmts=pmtHC->entries();
    fEvent->nPmtHits = pmts;
    fEvent->positionEWeight.Clear();
    G4ThreeVector reconPos(0,0,0);
    G4double totalWeight=0;
    G4double photonCalib = h_Planck*c_light/(400.0*nm);
    for(G4int i=0;i<pmts;i++) {
      G4int nPhotons = (*pmtHC)[i]->GetNumPhotons();
      fEvent->nPmtPhotons+= nPhotons;
      fEvent->ePmt += G4double(nPhotons)*photonCalib;
      G4double weight = pow(double(nPhotons),2.);
      totalWeight += weight;
      reconPos+=(*pmtHC)[i]->GetPos()*weight;
    }
    G4cout << "  ANALYSIS calib " <<  photonCalib << "  nPmtPhotons "  << fEvent->nPmtPhotons << "  ePmt "  << fEvent->ePmt << G4endl;

    reconPos/totalWeight;
    fEvent->positionEWeight.SetXYZ( reconPos.x(), reconPos.y(), reconPos.z());
  }

  // and end of analysis save this event
  fEvent->print();
  fTree->Fill();
  //printf(" +++++++++++++++++++ Leaving Legend Analysis +++++++++++++++++++++++++++++ \n");
}
   
void  LegendAnalysis::anaTrajectories(G4TrajectoryContainer* trajectoryContainer)
{
  fEvent->nTraj = trajectoryContainer->entries();
  for(int ij=0; ij < fEvent->nTraj; ++ij) {
    G4VTrajectory* gtrj =  (*trajectoryContainer)[ij];
    LTTraject ltraj;
    // fill from trajectory 
    ltraj.TrajId = gtrj->GetTrackID() ;  
    ltraj.ParentId = gtrj->GetParentID();        
    //PrimaryId = gtrj-> ;        
    ltraj.PDG = gtrj->GetPDGEncoding();       
    //Mass = gtrj-> ;   
    ltraj.Charge = gtrj->GetCharge(); 

    // each element in std::vector corresponds to a point on the particle path
    // typdef CLHEP::Hep3Vector G4ThreeVector;
    G4ThreeVector gpos0 = gtrj->GetPoint(0)->GetPosition();
    TLorentzVector r4(gpos0.x(),gpos0.y(),gpos0.z(),0);// were is the time?
    
    // G4int npoints = gtrj->GetPointEntries();
    /*for(G4int ip=0; ip<npoints ; ++ip ) {
      G4VTrajectoryPoint* gtrjp = gtrj->GetPoint(ip);
      G4ThreeVector GetPosition() 
    }*/

    ltraj.Position.push_back(r4);
    G4ThreeVector momentum3 = gtrj->GetInitialMomentum();
    TVector3 p3(momentum3.x(),momentum3.y(),momentum3.z());
    ltraj.KE = p3.Mag();// don't have mass
    ltraj.Momentum.push_back(p3);

    //G4cout <<  " ANALYSIS id " <<   ltraj.TrajId  << " parent "  << ltraj.ParentId << " PDG "  <<  ltraj.PDG << " charge " <<
    //  ltraj.Charge << G4endl;
    if(ltraj.Charge==0) {
      if(gtrj->GetParticleName()=="opticalphoton") fEvent->eOptical += ltraj.KE;
      else fEvent->eNeutral += ltraj.KE;
    } else {
      //gtrj->ShowTrajectory();
      fEvent->eCharged += ltraj.KE;
    }

    //G4cout << " ANALYSIS E charged " << fEvent->eCharged << " neutral  " <<  fEvent->eNeutral << " optical  " << fEvent->eOptical << G4endl;
    
    ltraj.Name = TString(gtrj->GetParticleName().data());
    
  
    if(gtrj->GetParticleName()=="opticalphoton") {
      G4double waveLength =  h_Planck*c_light/ltraj.KE/nm;//700 nm
      hWOptical->Fill(waveLength);
      if(waveLength<200) ++fEvent->nArScint;
      else  ++fEvent->nWlsScint;
    } else if(ltraj.PDG==11) { // electron
      hEElectron->Fill(ltraj.KE/keV);
    } else if(ltraj.PDG==22) { // gamma 
      hEGamma ->Fill(ltraj.KE/keV);
    } else
      G4cout << " LegendAnalysis UNKNOWN traj " << ij << "  " << gtrj->GetParticleName() << "  " << ltraj.PDG << " ke (KeV) " << ltraj.KE/keV  << G4endl;

    //std::vector<Int_t>   Region;
    //std::vector<LTHitSegment> segments;
    
    // save in tree
    fEvent->traject.push_back(ltraj);
  }
}
