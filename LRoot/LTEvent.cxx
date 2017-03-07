#include "LTEvent.hxx"
ClassImp(LTEvent)

LTEvent::LTEvent(): TNamed("LTEvent","LTEvent")
{
  clear();
}

LTEvent::~LTEvent()
{
}

void LTEvent::clear(){
  HitCount=0;
  PhotonCount_Scint=0;
  PhotonCount_Ceren=0;
  AbsorptionCount=0;
  BoundaryAbsorptionCount=0;
  TotE=0;
  EWeightPos.Clear();
  ReconPos.Clear(); //Also relies on hitCount>0
  ConvPos.Clear();//true (initial) converstion position
  PosMax.Clear();
  bool ConvPosSet=false;
  EdepMax=0;
}
