/*
** Interpolate points in this graph at x using a TSpline
  -if spline==0 and option="" a linear interpolation between the two points
   close to x is computed. If x is outside the graph range, a linear
   extrapolation is computed.
  -if spline==0 and option="S" a TSpline3 object is created using this graph
   and the interpolated value from the spline is returned.
   the internally created spline is deleted on return.
  -if spline is specified, it is used to return the interpolated value.
*/
// LegendDetectorConstruction constant LambdaE =1.23984e-09

void test(int ntry=1000) 
{
  TRandom3 *ran = new TRandom3();
  TFile *tpbFile = new TFile("tpbGhemann.root");
  TGraph* fTPBspec=NULL;
  tpbFile->ls();
  tpbFile->GetObject("tpbGhemann",fTPBspec);
  TCanvas *c1 = new TCanvas("tpbGhemann","tpbGhemann");
  fTPBspec->Draw();
  TH1F *htpb = new TH1F("htpb"," tpb efficiency ",100,350,650);
  TH1F *htpb2 = new TH1F("htpb2"," tpb efficiency by bin ",1000,350,650);
  for(int i=0; i<ntry ; ++i) {
    float lambda = 350.0 + (650.0-350.0)*ran->Rndm();
    float eff = fTPBspec->Eval(lambda);
    //printf(" %i %f %f \n",i,lambda,eff);
    htpb->Fill(lambda,eff);
  }
  TCanvas *c2 = new TCanvas("tpb-random","tpb-random");
  htpb->Draw();

  for(int i=0; i<htpb2->GetNbinsX()  ; ++i) {
    float ell = htpb2->GetBinCenter(i);
    htpb2->SetBinContent(i,fTPBspec->Eval(ell));
  }
  
  TCanvas *c3 = new TCanvas("tpb-bins","tpb-bins");
  htpb2->Draw();
  
}
