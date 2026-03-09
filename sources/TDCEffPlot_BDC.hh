#ifndef TDCEFFPLOT_BDC_HH
#define TDCEFFPLOT_BDC_HH

#include "TDCEffPlot.hh"
#include "TGraph.h"
#include "TFile.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"

class TDCEffPlot_BDC : public TDCEffPlot
{
public:
  TDCEffPlot_BDC(Int_t iBDC=1);
  virtual ~TDCEffPlot_BDC();

  virtual void AnalyzeRun(Int_t nRun, Long64_t neve=0);
  virtual void Plot(Int_t layer);

  virtual void PlotSummary(TString dir="X");
  
protected:
  Int_t fBDCid;
  TArtSAMURAIParameters* smprm;
  TArtCalibBDC1Hit *fCalibBDC1Hit;
  TArtCalibBDC2Hit *fCalibBDC2Hit;
  
  static const int kMmax = 6;

  int fNwire;
  
  //TList *fPlotItems;
  
  ClassDef(TDCEffPlot_BDC, 1)
};

#endif
