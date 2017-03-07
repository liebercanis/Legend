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
using namespace std;


// class to store info for the event

class LTEvent: public TNamed {
	public:
		LTEvent();
		~LTEvent();

    void clear();
    // data elements

    Int_t HitCount;
    Int_t PhotonCount_Scint;
    Int_t PhotonCount_Ceren;
    Int_t AbsorptionCount;
    Int_t BoundaryAbsorptionCount;

    Double_t TotE;

    //These only have meaning if totE > 0
    //If totE = 0 then these wont be set by EndOfEventAction
    TVector3 EWeightPos;
    TVector3 ReconPos; //Also relies on hitCount>0
    TVector3 ConvPos;//true (initial) converstion position
    bool ConvPosSet;
    TVector3 PosMax;
    Double_t EdepMax;

 		
ClassDef(LTEvent,1)
};
#endif

