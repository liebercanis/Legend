/**
** MG, February 2017 
**/
#ifndef LTEVENT_DEFINED
#define LTEVENT_DEFINED
#include <iostream>
#include <string>
#include <TNamed.h>
#include <TTree.h>
#include <TVector3.h>
#include <vector>

// LRoot classes
#include "LTPVertex.hxx"
#include "LTTraject.hxx"

using namespace std;


// class to store info for the event

class LTEvent: public TNamed {
	public:
		LTEvent();
		~LTEvent();

    void clear();
    void print();
    // data elements

    Int_t evId;    // event id
    Int_t nPVert; // number primary verticies
    vector<LTPVertex> pvertex;
    Int_t nTraj;
    vector<LTTraject> traject;

    Int_t nPmtHits;
    Int_t nPmtPhotons;
    Int_t nArScint;
    Int_t nWlsScint;
    Int_t nCherenkov;
    Int_t nAbsorbed;
    Int_t nAbsorbedBoundary;

    Int_t PDG;
    Double_t ePrimary;
    Double_t eCharged;
    Double_t eNeutral;
    Double_t eOptical;
    Double_t ePmt;
    Double_t eMaxDeposit;

    bool hasConversion;//true (initial) converstion position
    TVector3 positionEWeight;
    TVector3 positionConversion;
    TVector3 positionMax;

 		
ClassDef(LTEvent,1)
};
#endif

