#ifndef TDCEFFPLOT_FDC_HH
#define TDCEFFPLOT_FDC_HH

#include "TDCEffPlot.hh"
#include "TGraph.h"
#include "TFile.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Hit.hh"

class TDCEffPlot_FDC : public TDCEffPlot
{
public:
  TDCEffPlot_FDC(Int_t iFDC=1);
  virtual ~TDCEffPlot_FDC();

  virtual void AnalyzeRun(Int_t nRun, Long64_t neve=0);
  virtual void Plot(Int_t layer);

  virtual void PlotSummary(TString dir="X");
  
protected:
  Int_t fFDCid;
  TArtSAMURAIParameters* smprm;
  TArtCalibFDC1Hit *fCalibFDC1Hit;
  TArtCalibFDC2Hit *fCalibFDC2Hit;
  
  static const int kMmax = 6;

  TList *fPlotItems;
  
  ClassDef(TDCEffPlot_FDC, 1)
};

#endif
